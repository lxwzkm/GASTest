// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecuCalcu_Damage.generated.h"

/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API UExecuCalcu_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UExecuCalcu_Damage();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
