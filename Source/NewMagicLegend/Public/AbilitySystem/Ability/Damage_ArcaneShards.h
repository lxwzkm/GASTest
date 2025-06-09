// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/DamageGameplayAbility.h"
#include "Damage_ArcaneShards.generated.h"

/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API UDamage_ArcaneShards : public UDamageGameplayAbility
{
	GENERATED_BODY()
public:
	virtual FString GetCurrentLevelDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	int32 MaxSpawnNum=11;
};
