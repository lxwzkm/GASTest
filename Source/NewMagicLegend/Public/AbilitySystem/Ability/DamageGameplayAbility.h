// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
protected:
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect>DamageEffectClass;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	TMap<FGameplayTag,FScalableFloat>DamageTypes;

	UFUNCTION(BlueprintPure)
	FTagMontage GetRandomTagMontage(const TArray<FTagMontage>& TagMontages);
};
