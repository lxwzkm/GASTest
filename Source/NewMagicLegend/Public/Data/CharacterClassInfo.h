// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScalableFloat.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

class UGameplayAbility;
class UGameplayEffect;

/**
 * @brief 玩家角色的种类
 */
UENUM(BlueprintType)
enum class ECharacterClass
{
	Elementalist,
	Warrior,
	Ranger
};

/**
 * @brief 玩家不同种类独有的属性
 */
USTRUCT(BlueprintType)
struct FCharacterAttribute
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,Category="ClassDefaultsInfo")
	TSubclassOf<UGameplayEffect>PrimaryAttribute;

	//每个职业不同的起始技能
	UPROPERTY(EditDefaultsOnly,Category="ClassDefaultsInfo")
	TArray<TSubclassOf<UGameplayAbility>>StartupAbility;

	UPROPERTY(EditDefaultsOnly,Category="ClassDefaultsInfo")
	FScalableFloat XPReward;
};

/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly,Category="CharacterClassInfo")
	TMap<ECharacterClass,FCharacterAttribute>CharacterClassDefaultsInfo;
	
	//玩家共有的属性
	UPROPERTY(EditDefaultsOnly,Category="Common CharacterAttribute")
	TSubclassOf<UGameplayEffect>SecondaryAttribute;

	UPROPERTY(EditDefaultsOnly,Category="Common CharacterAttribute")
	TSubclassOf<UGameplayEffect>VitalAttribute;

	UPROPERTY(EditDefaultsOnly,Category="Common Ability")
	TArray<TSubclassOf<UGameplayAbility>>CommonAbility;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	TObjectPtr<UCurveTable>DamageCoefficient;

	FCharacterAttribute GetCharacterAttribute(ECharacterClass CharacterClass);
};
