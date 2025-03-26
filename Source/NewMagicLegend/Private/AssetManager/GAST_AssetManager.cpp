// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetManager/GAST_AssetManager.h"
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
}
