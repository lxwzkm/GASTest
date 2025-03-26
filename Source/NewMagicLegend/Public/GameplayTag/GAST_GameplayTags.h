// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * 单例模式的GameplayTag
 */

struct FGameplayTags
{
public:
	static const FGameplayTags& Get(){return GameplayTags;}
	static void InitializeGameplayTags();

	FGameplayTag Attributes_Secondary_Armor;
private:
	static FGameplayTags GameplayTags;
};

FGameplayTags FGameplayTags::GameplayTags;