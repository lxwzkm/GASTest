// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbility.h"
#include "GAST_GameplayAbilityBase.generated.h"
/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API UGAST_GameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly,Category="Tag")
	FGameplayTag StartupInputTag;


};
