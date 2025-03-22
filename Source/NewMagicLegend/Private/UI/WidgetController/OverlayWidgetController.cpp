// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/GAST_AttributeSet.h"

void UOverlayWidgetController::BroadcastInitValues()
{
	UGAST_AttributeSet*AttributeSet= Cast<UGAST_AttributeSet>(AS);
	check(AttributeSet);
	
	OnHealthChanged.Broadcast(AttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	UGAST_AttributeSet*AttributeSet= Cast<UGAST_AttributeSet>(AS);
	/*---------------------------  属性变化绑定  ----------------------------*/
	ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddLambda(
   [this](const FOnAttributeChangeData& Data)
   {
   	    OnHealthChanged.Broadcast(Data.NewValue);
   }
	);
	
	ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute()).AddLambda(
    [this](const FOnAttributeChangeData& Data)
    {
    	OnMaxHealthChanged.Broadcast(Data.NewValue);
    }
	);
	
	ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetManaAttribute()).AddLambda(
    [this](const FOnAttributeChangeData& Data)
    {
    	OnManaChanged.Broadcast(Data.NewValue);
    }
	);
	
	ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxManaAttribute()).AddLambda(
    [this](const FOnAttributeChangeData& Data)
    {
    	OnMaxManaChanged.Broadcast(Data.NewValue); 
    }
	);
	/*---------------------------  发消息给UI  ----------------------------*/
	if (UGAST_AbilitySystemComponent*GAST_ASC=Cast<UGAST_AbilitySystemComponent>(ASC))
	{
		GAST_ASC->AllAssetTagsContainerDelegate.AddLambda(
         [this](const FGameplayTagContainer& TagContainer)
         {
         	for (const auto& tag:TagContainer)
         	{
         		FGameplayTag MessageTag= FGameplayTag::RequestGameplayTag(FName("Message"));
         		if (tag.MatchesTag(MessageTag))
         		{
         			FMessageDataRow* Row= GetDataTableRowByTag<FMessageDataRow>(MessageData,tag);
         			SendUIMessageDelegate.Broadcast(*Row);
         		}
         	}
         }
		);
	}
}