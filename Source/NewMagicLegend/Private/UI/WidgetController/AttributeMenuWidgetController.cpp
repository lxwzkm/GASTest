// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/GAST_AttributeSet.h"
#include "GameplayTag/GAST_GameplayTags.h"

void UAttributeMenuWidgetController::BroadcastInitValues()
{
	UGAST_AttributeSet* AttributeSet=Cast<UGAST_AttributeSet>(AS);

	check(AttributeInformation);
	FGAST_AttributeInfo AttributeInfo= AttributeInformation->GetAttributeInfoByTag(FGameplayTags::Get().Attributes_Primary_Strength);
	AttributeInfo.AttributeValue=AttributeSet->GetStrength();

	AttributeInfoDelegate.Broadcast(AttributeInfo);
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	
}
