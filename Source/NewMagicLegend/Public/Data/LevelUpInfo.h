// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

struct FScalableFloat;

USTRUCT(BlueprintType)
struct FMyLevelUpInfo
{
	GENERATED_BODY()

	//当前等级升级所需的经验 数组0为占位符，无实际意义,经验为累计计算方式，当前等级减去前一个等级的经验才是本级升级所需的经验
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="LevelUpInfo")
	int32 LevelUpRequirement=0;

	//升级奖励的属性点
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="LevelUpInfo")
	int32 AttributePointsReward=4;

	//升级奖励的技能点
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="LevelUpInfo")
	int32 SpellPointsReward=2;
};

/**
 * 
 */
UCLASS(BlueprintType)
class NEWMAGICLEGEND_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="LevelUpInfo")
	TArray<FMyLevelUpInfo>LevelUpInfos;

	UFUNCTION(BlueprintCallable)
	int32 FindLevelForXP(int32 XP);
};
