// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/Ability_ProjectileSpell.h"
#include "Projectile_FireBolt.generated.h"

/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API UProjectile_FireBolt : public UAbility_ProjectileSpell
{
	GENERATED_BODY()
public:
	virtual FString GetCurrentLevelDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UFUNCTION(BlueprintCallable)
	void SpawnPrijectiles(const FVector& TargetLocation,const FGameplayTag& CombatSocketTag,bool bOverridePitch,float PitchOverride,AActor* HomingTarget);
protected:

	UPROPERTY(EditAnywhere)
	float ProjectileSpread=90.f;

	UPROPERTY(EditAnywhere)
	int32 MaxSpawnNum=5;

	UPROPERTY(EditAnywhere)
	float HomingAccelerateMin=1600;
	UPROPERTY(EditAnywhere)
	float HomingAccelerateMax=3200;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent>SceneComponent;
};
