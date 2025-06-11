// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/DamageGameplayAbility.h"
#include "Damage_FireBlast.generated.h"

class AProjectile_FIreBall;
/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API UDamage_FireBlast : public UDamageGameplayAbility
{
	GENERATED_BODY()
public:
	virtual FString GetCurrentLevelDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UFUNCTION(BlueprintCallable)
	TArray<AProjectile_FIreBall*> SpawnProjectile_FIreBalls();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int32 MaxSpawnNum=12;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectile_FIreBall> FireBallClass;
};
