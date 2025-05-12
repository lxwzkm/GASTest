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
};
