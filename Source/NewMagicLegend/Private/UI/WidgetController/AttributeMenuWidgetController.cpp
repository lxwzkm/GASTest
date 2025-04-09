// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/GAST_AttributeSet.h"
#include "GameplayTag/GAST_GameplayTags.h"

void UAttributeMenuWidgetController::BroadcastInitValues()
{
	UGAST_AttributeSet* AttributeSet=CastChecked<UGAST_AttributeSet>(AS);

	check(AttributeInformation);
	for (auto& Pair:AttributeSet->TagsToAttribute)
	{
		BroadcastAttributeInfo(Pair.Value(),Pair.Key);
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UGAST_AttributeSet* AttributeSet=CastChecked<UGAST_AttributeSet>(AS);

	for (auto& Pair:AttributeSet->TagsToAttribute)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
[this,Pair,AttributeSet](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Value(),Pair.Key);
			}
		);
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayAttribute& Attribute,
	const FGameplayTag& GameplayTag) const
{
	FGAST_AttributeInfo AttributeInfo= AttributeInformation->GetAttributeInfoByTag(GameplayTag);
	AttributeInfo.AttributeValue=Attribute.GetNumericValue(AS);
	AttributeInfoDelegate.Broadcast(AttributeInfo);
}
