// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayTag/GAST_GameplayTags.h"
#include "GameplayTagsManager.h"

void FGameplayTags::InitializeGameplayTags()
{
	GameplayTags.Attributes_Secondary_Armor=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"),FString("护甲"));
}
