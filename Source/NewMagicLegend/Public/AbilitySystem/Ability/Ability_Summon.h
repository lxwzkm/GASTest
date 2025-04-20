// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/GAST_GameplayAbilityBase.h"
#include "Ability_Summon.generated.h"

/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API UAbility_Summon : public UGAST_GameplayAbilityBase
{
	GENERATED_BODY()
public:
	/**
	 * @brief 获取生成召唤物的随机位置
	 */
	UFUNCTION(BlueprintCallable,Category="Summom")
	TArray<FVector> GetSpawnLocations();

	UFUNCTION(BlueprintPure,Category="Summom")
	TSubclassOf<APawn> GetRandomMinionClass();

	/*---------------------------- 召唤相关的变量 ----------------------------*/
	//召唤物的种类
	UPROPERTY(EditDefaultsOnly,Category="Summom")
	TArray<TSubclassOf<APawn>> MinionClasses;

	//召唤物距召唤者的最近距离
	UPROPERTY(EditDefaultsOnly,Category="Summom")
	float MinDistance=100.f;

	//召唤物距召唤者的最远距离
	UPROPERTY(EditDefaultsOnly,Category="Summom")
	float MaxDistance=400.f;

	//召唤物生成位置与召唤者的角度
	UPROPERTY(EditDefaultsOnly,Category="Summom")
	float SpawnSpread=90.f;

	//召唤的数量
	UPROPERTY(EditDefaultsOnly,Category="Summom")
	int32 NumMinions=5;
};
