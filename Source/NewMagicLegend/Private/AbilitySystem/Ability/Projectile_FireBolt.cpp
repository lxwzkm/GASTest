// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Projectile_FireBolt.h"

#include "GAST_AbilitySystemLibrary.h"
#include "Actor/GAST_Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameplayTag/GAST_GameplayTags.h"
#include "Kismet/KismetSystemLibrary.h"

FString UProjectile_FireBolt::GetCurrentLevelDescription(int32 Level)
{
	const float Damage = DamageValue.GetValueAtLevel(Level);
	const float Cost=GetManaCost(Level);
	const float CD=GetCooldown(Level);
	FString Description=FString::Printf(TEXT(
		"<Title>火球术</>\n\n"
		"<Default>当前等级： </><Level>%d</>\n"
		"<Default>当前法力消耗:</><ManaCost> %.1f </>\n"
		"<Default>当前CD:</><Cooldown> %.1f </>\n\n"
		"<Default>发射 %d 个火球，"
		"造成火系伤害</><Damage> %.1f </>\n"
		"<Default>火系伤害有一定几率造成燃烧效果</>"
		),Level,FMath::Abs(Cost),CD,FMath::Min(Level,MaxSpawnNum),Damage);
	return Description;
}

FString UProjectile_FireBolt::GetNextLevelDescription(int32 Level)
{
	const float Damage = DamageValue.GetValueAtLevel(Level);
	const float Cost=GetManaCost(Level);
	const float CD=GetCooldown(Level);
	FString Description=FString::Printf(TEXT(
		"<Title>火球术</>\n\n"
		"<Default>下一等级： </><Level>%d</>\n"
		"<Default>法力消耗:</><ManaCost> %.1f </>\n"
		"<Default>CD:</><Cooldown> %.1f </>\n\n"
		"<Default>发射 %d 个火球，"
		"造成火系伤害</><Damage> %.1f </>\n"
		"<Default>火系伤害有一定几率造成燃烧效果</>"
		),Level,FMath::Abs(Cost),CD,FMath::Min(Level,MaxSpawnNum),Damage);
	return Description;
}

void UProjectile_FireBolt::SpawnPrijectiles(const FVector& TargetLocation, const FGameplayTag& CombatSocketTag,bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	bool IsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!IsServer)return;
	
	const FVector SocketLocation=ICombatInterface::Execute_GetWeaponSocketLocation(GetAvatarActorFromActorInfo(),CombatSocketTag);
	
	FRotator Rotation=(TargetLocation-SocketLocation).Rotation();
	if (bOverridePitch)
	{
		Rotation.Pitch=PitchOverride;
	}
	int32 EffectProjectileNum=FMath::Min(MaxSpawnNum,GetAbilityLevel());
	const FVector ForWard=Rotation.Vector();
	TArray<FRotator> Rotators=UGAST_AbilitySystemLibrary::EvenlySpaceRotators(ForWard,FVector::UpVector,ProjectileSpread,EffectProjectileNum);
	for (FRotator Rot:Rotators)
	{
		FTransform SpawnTranform;
		SpawnTranform.SetLocation(SocketLocation);
		SpawnTranform.SetRotation(Rot.Quaternion());
	
		AGAST_Projectile*ProjectileSpawn= GetWorld()->SpawnActorDeferred<AGAST_Projectile>(
			ProjectileClass,SpawnTranform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
		ProjectileSpawn->DamageEffectParams=MakeDamageParams(nullptr);

		if (HomingTarget && HomingTarget->Implements<UCombatInterface>())
		{
			ProjectileSpawn->ProjectileMovementComponent->HomingTargetComponent=HomingTarget->GetRootComponent();
		
		}
		else
		{
			SceneComponent=NewObject<USceneComponent>(USceneComponent::StaticClass());
			SceneComponent->SetWorldLocation(TargetLocation);
			ProjectileSpawn->ProjectileMovementComponent->HomingTargetComponent=SceneComponent;
		}
		ProjectileSpawn->ProjectileMovementComponent->HomingAccelerationMagnitude=FMath::FRandRange(HomingAccelerateMin,HomingAccelerateMax);
		ProjectileSpawn->ProjectileMovementComponent->bIsHomingProjectile=true;
		
		ProjectileSpawn->FinishSpawning(SpawnTranform);
	}
}
