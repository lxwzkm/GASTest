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

void UAbility_ProjectileSpell::SpawnPrijectile(const FVector& TargetLocation,const FGameplayTag& CombatSocketTag,bool bOverridePitch,float PitchOverride)
{
	bool IsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!IsServer)return;
	
	const FVector SocketLocation=ICombatInterface::Execute_GetWeaponSocketLocation(GetAvatarActorFromActorInfo(),CombatSocketTag);
	
	FRotator Rotation=(TargetLocation-SocketLocation).Rotation();
	if (bOverridePitch)
	{
		Rotation.Pitch=PitchOverride;
	}
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

FString UAbility_ProjectileSpell::GetCurrentLevelDescription(int32 Level)
{
	const float Damage= GetDamageByDamageTypes(Level,FGameplayTags::Get().Damage_Fire);
	const float Cost=GetManaCost(Level);
	const float CD=GetCooldown(Level);
	FString Description=FString::Printf(TEXT(
		"<Title>火球术</>\n\n"
		"<Default>当前等级：</><Level>%d</>\n"
		"<Default>当前法力消耗:</><ManaCost>%.2f</>\n"
		"<Default>当前CD:</><Cooldown>%.1f</>\n\n"
		"<Default>发射%d个火球，"
		"造成火系伤害</><Damage>%.2f</>"
		"<Default>火系伤害有一定几率造成燃烧效果</>"
		),Level,FMath::Abs(Cost),CD,Level,Damage);
	return Description;
}

FString UAbility_ProjectileSpell::GetNextLevelDescription(int32 Level)
{
	const float Damage= GetDamageByDamageTypes(Level,FGameplayTags::Get().Damage_Fire);
	const float Cost=GetManaCost(Level);
	const float CD=GetCooldown(Level);
	FString Description=FString::Printf(TEXT(
		"<Title>火球术</>\n\n"
		"<Default>下一等级：</><Level>%d</>\n"
		"<Default>法力消耗:</><ManaCost>%.2f</>\n"
		"<Default>CD:</><Cooldown>%.1f</>\n\n"
		"<Default>发射%d个火球，"
		"造成火系伤害</><Damage>%.2f</>"
		"<Default>火系伤害有一定几率造成燃烧效果</>"
		),Level,FMath::Abs(Cost),CD,FMath::Clamp(Level,1,3),Damage);
	return Description;
}
