// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAST_AbilityType.h"
#include "AbilitySystem/Ability/GAST_GameplayAbilityBase.h"
#include "Interaction/CombatInterface.h"
#include "DamageGameplayAbility.generated.h"


/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API UDamageGameplayAbility : public UGAST_GameplayAbilityBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void CauseDamageToTarget(AActor* TargetActor);

	FDamageEffectParams MakeDamageParams(AActor* TargetActor);
protected:
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect>DamageEffectClass;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	FGameplayTag DamageType;
	UPROPERTY(EditDefaultsOnly,Category="Damage")
	FScalableFloat DamageValue;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	float Debuff_Chance=20;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	float Debuff_Damage=5;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	float Debuff_Duration=5;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	float Debuff_Frequency=1;

	UFUNCTION(BlueprintPure)
	FTagMontage GetRandomTagMontage(const TArray<FTagMontage>& TagMontages);
	
};
