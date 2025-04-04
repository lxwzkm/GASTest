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
	const FGameplayEffectSpecHandle SpecHandle= SourceASC->MakeOutgoingSpec(DamageEffectClass,GetAbilityLevel(),SourceASC->MakeEffectContext());

	FGameplayTags GameplayTags= FGameplayTags::Get();
	const float ScaleDamage= Damage.GetValueAtLevel(40);
	
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,GameplayTags.Damage,ScaleDamage);
	ProjectileSpawn->DamageSpecHandle=SpecHandle;

	ProjectileSpawn->FinishSpawning(SpawnTranform);
}
