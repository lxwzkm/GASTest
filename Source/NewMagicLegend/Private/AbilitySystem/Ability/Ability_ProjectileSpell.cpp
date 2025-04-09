// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Ability_ProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/GAST_Projectile.h"
#include "GameplayTag/GAST_GameplayTags.h"
#include "Interaction/CombatInterface.h"

void UAbility_ProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                               const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


}

void UAbility_ProjectileSpell::SpawnPrijectile(const FVector& TargetLocation)
{
	bool IsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!IsServer)return;

	ICombatInterface* CombatInterface=Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	const FVector SocketLocation=CombatInterface->GetWeaponSocketLocation();
	
	
	FRotator Rotation=(TargetLocation-SocketLocation).Rotation();
	Rotation.Pitch=0.f;
	FTransform SpawnTranform;
	SpawnTranform.SetLocation(SocketLocation);
	SpawnTranform.SetRotation(Rotation.Quaternion());
	
	AGAST_Projectile*ProjectileSpawn= GetWorld()->SpawnActorDeferred<AGAST_Projectile>(
		ProjectileClass,SpawnTranform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
	const UAbilitySystemComponent*SourceASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	FGameplayEffectContextHandle ContextHandle=SourceASC->MakeEffectContext();
	
	//尝试填充Context
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(ProjectileSpawn);
	TArray<TWeakObjectPtr<AActor>>Actors;
	ContextHandle.AddActors(Actors);
	FHitResult Hit;
	Hit.Location=TargetLocation;
	ContextHandle.AddHitResult(Hit);
	
	const FGameplayEffectSpecHandle SpecHandle= SourceASC->MakeOutgoingSpec(DamageEffectClass,GetAbilityLevel(),ContextHandle);
	
	for (auto & pair:DamageTypes)
	{
		const float ScaleDamage= pair.Value.GetValueAtLevel(20);
		/** Sets a gameplay tag Set By Caller magnitude value  将Tag与伤害绑定在一起，存在spechandle里  SetByCaller是一个键值对 */
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,pair.Key,ScaleDamage);
	}
	
	ProjectileSpawn->DamageSpecHandle=SpecHandle;
	ProjectileSpawn->FinishSpawning(SpawnTranform);
}
