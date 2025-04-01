// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetManager/GAST_AssetManager.h"

#include "AbilitySystemGlobals.h"
#include "GameplayTag/GAST_GameplayTags.h"

UGAST_AssetManager& UGAST_AssetManager::Get()
{
	check(GEngine);
	UGAST_AssetManager* AssetManager= Cast<UGAST_AssetManager>(GEngine->AssetManager);
	return *AssetManager;
}

void UGAST_AssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FGameplayTags::InitializeGameplayTags();

	//初始化TargetData  5.4版本不需要调了
	UAbilitySystemGlobals::Get().InitGlobalData();
}
