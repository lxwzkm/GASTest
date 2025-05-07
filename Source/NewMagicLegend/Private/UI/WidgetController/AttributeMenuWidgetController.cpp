// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/GAST_AttributeSet.h"
#include "Gamemode/GAST_PlayerState.h"
#include "GameplayTag/GAST_GameplayTags.h"

void UAttributeMenuWidgetController::BroadcastInitValues()
{
	
	check(AttributeInformation);
	for (auto& Pair:GetAttributeSet()->TagsToAttribute)
	{
		BroadcastAttributeInfo(Pair.Value(),Pair.Key);
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{

	for (auto& Pair:GetAttributeSet()->TagsToAttribute)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
[this,Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Value(),Pair.Key);
			}
		);
	}

	GetPlayerState()->OnSpellPointsChangeDelegate.AddLambda(
[this](int32 NewSpellPoints)
	{
		OnSpellPointsChangeDelegate.Broadcast(NewSpellPoints);
	}
);
	GetPlayerState()->OnAttributePointsChangeDelegate.AddLambda(
[this](int32 NewAttributePoints)
	{
		OnAttributePointsChangeDelegate.Broadcast(NewAttributePoints);
	}
);
}

void UAttributeMenuWidgetController::UpgradeAttributePoints(const FGameplayTag& AttributeTag)
{
	if (GetAbilitySystemComponent())
	{
		GetAbilitySystemComponent()->UpgradeAttributePoints(AttributeTag);
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayAttribute& Attribute,
                                                            const FGameplayTag& GameplayTag)
{
	FGAST_AttributeInfo AttributeInfo= AttributeInformation->GetAttributeInfoByTag(GameplayTag);
	AttributeInfo.AttributeValue=Attribute.GetNumericValue(AS);
	AttributeInfoDelegate.Broadcast(AttributeInfo);

	OnAttributePointsChangeDelegate.Broadcast(GetPlayerState()->GetAttributePoints());
	OnSpellPointsChangeDelegate.Broadcast(GetPlayerState()->GetSpellPoints());
}
