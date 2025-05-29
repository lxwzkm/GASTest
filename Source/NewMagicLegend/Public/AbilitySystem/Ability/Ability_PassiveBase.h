// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/GAST_GameplayAbilityBase.h"
#include "Ability_PassiveBase.generated.h"

/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API UAbility_PassiveBase : public UGAST_GameplayAbilityBase
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void ReciveDeactive(const FGameplayTag& AbilityTag);
};
