// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/DamageGameplayAbility.h"
#include "Ability_ProjectileSpell.generated.h"

class AGAST_Projectile;
class UGameplayEffect;

/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API UAbility_ProjectileSpell : public UDamageGameplayAbility
{
	GENERATED_BODY()
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/**
	 * 生成发射物的函数，蓝图调用
	 */
	UFUNCTION(BlueprintCallable)
	void SpawnPrijectile(const FVector& TargetLocation);
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<AGAST_Projectile> ProjectileClass;
	
};
