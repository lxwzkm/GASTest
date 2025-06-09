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

	UFUNCTION(BlueprintPure)
	FDamageEffectParams MakeDamageParams(AActor* TargetActor=nullptr,FVector RadialDamageOrigin=FVector::ZeroVector,
		bool bKnockBackOverride=false,
		FVector KnockBackDirectionOverride=FVector::ZeroVector,
		bool bDeathImpluseOverride=false,
		FVector DeathDirectionOverride=FVector::ZeroVector,
		bool bPitchOverride=false,
		float PitchOverride=0.0f);

	UFUNCTION(BlueprintPure)
	float GetDamageAtLevel();
protected:
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect>DamageEffectClass;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	FGameplayTag DamageType;
	UPROPERTY(EditDefaultsOnly,Category="Damage")
	FScalableFloat DamageValue;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	float Debuff_Chance=20.f;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	float Debuff_Damage=5.f;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	float Debuff_Duration=5.f;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	float Debuff_Frequency=1.f;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	float DeathImpulseMagnitude=60.f;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	float KnockBackMagnitude=1000.f;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	float KnockBackChance=0.f;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	bool bIsRadialDamge=false;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Damage")
	float RadialDamageInnerRadius=0.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Damage")
	float RadialDamageOuterRadius=0.f;

	UFUNCTION(BlueprintPure)
	FTagMontage GetRandomTagMontage(const TArray<FTagMontage>& TagMontages);
	
};
