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
	
	ProjectileSpawn->DamageEffectParams=MakeDamageParams(nullptr);
	ProjectileSpawn->FinishSpawning(SpawnTranform);
}