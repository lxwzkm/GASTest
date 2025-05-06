// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/GAST_AttributeSet.h"
#include "Gamemode/GAST_PlayerState.h"
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
	AGAST_PlayerState* PlayerState=CastChecked<AGAST_PlayerState>(PS);

	for (auto& Pair:AttributeSet->TagsToAttribute)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
[this,Pair,AttributeSet](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Value(),Pair.Key);
			}
		);
	}

	PlayerState->OnSpellPointsChangeDelegate.AddLambda(
[this](int32 NewSpellPoints)
	{
		OnSpellPointsChangeDelegate.Broadcast(NewSpellPoints);
	}
);
	PlayerState->OnAttributePointsChangeDelegate.AddLambda(
[this](int32 NewAttributePoints)
	{
		OnAttributePointsChangeDelegate.Broadcast(NewAttributePoints);
	}
);
}

void UAttributeMenuWidgetController::UpgradeAttributePoints(const FGameplayTag& AttributeTag)
{
	if (UGAST_AbilitySystemComponent* MyASC=Cast<UGAST_AbilitySystemComponent>(ASC))
	{
		MyASC->UpgradeAttributePoints(AttributeTag);
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayAttribute& Attribute,
                                                            const FGameplayTag& GameplayTag) const
{
	FGAST_AttributeInfo AttributeInfo= AttributeInformation->GetAttributeInfoByTag(GameplayTag);
	AttributeInfo.AttributeValue=Attribute.GetNumericValue(AS);
	AttributeInfoDelegate.Broadcast(AttributeInfo);

	AGAST_PlayerState* PlayerState = Cast<AGAST_PlayerState>(PS);

	OnAttributePointsChangeDelegate.Broadcast(PlayerState->GetAttributePoints());
	OnSpellPointsChangeDelegate.Broadcast(PlayerState->GetSpellPoints());
}
