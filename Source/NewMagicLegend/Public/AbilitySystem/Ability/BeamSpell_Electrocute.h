// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/Damage_BeamSpell.h"
#include "BeamSpell_Electrocute.generated.h"

/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API UBeamSpell_Electrocute : public UDamage_BeamSpell
{
	GENERATED_BODY()
public:
	virtual FString GetCurrentLevelDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
};
