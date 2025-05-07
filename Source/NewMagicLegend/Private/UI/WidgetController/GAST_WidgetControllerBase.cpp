// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/GAST_WidgetControllerBase.h"

#include "AbilitySystem/GAST_AttributeSet.h"
#include "Data/MyAbilityInfo.h"
#include "Gamemode/GAST_PlayerCOntroller.h"
#include "Gamemode/GAST_PlayerState.h"

void UGAST_WidgetControllerBase::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PC=WCParams.PlayerController;
	PS=WCParams.PlayerState;
	AS=WCParams.AttributeSet;
	ASC=WCParams.AbilitySystemComponent;
}

void UGAST_WidgetControllerBase::BroadcastInitValues()
{
	
}

void UGAST_WidgetControllerBase::BindCallbacksToDependencies()
{
	
}

AGAST_PlayerState* UGAST_WidgetControllerBase::GetPlayerState()
{
	if (MyPlayerState==nullptr)
	{
		MyPlayerState= Cast<AGAST_PlayerState>(PS);
	}
	return MyPlayerState;
}

UGAST_AbilitySystemComponent* UGAST_WidgetControllerBase::GetAbilitySystemComponent()
{
	if (MyAbilitySystemComponent == nullptr)
	{
		MyAbilitySystemComponent=Cast<UGAST_AbilitySystemComponent>(ASC);
	}
	return MyAbilitySystemComponent;
}

UGAST_AttributeSet* UGAST_WidgetControllerBase::GetAttributeSet()
{
	if (MyAttributeSet == nullptr)
	{
		 MyAttributeSet=Cast<UGAST_AttributeSet>(AS);
	}
	return MyAttributeSet;
}

AGAST_PlayerCOntroller* UGAST_WidgetControllerBase::GetPlayerController()
{
	if (MyPlayerController == nullptr)
	{
		 MyPlayerController=Cast<AGAST_PlayerCOntroller>(PC);
	}
	return MyPlayerController;
}

void UGAST_WidgetControllerBase::BroadcastAbilityInfo()
{
	if (!GetAbilitySystemComponent()->bGivenAbility)return;

	FForEachAbility AbilityDelegate;
	AbilityDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		//TODO 拿到AbilitySpec以后，要获取他们的AbilityInfo
		FGameplayTag AbilityTag = MyAbilitySystemComponent->GetGameplayTagByAbilitySpec(AbilitySpec);
		FAAbilityInfo AbilityInfo = AbilityInformation->GetMyAbilityInfoByAbilityTag(AbilityTag);
		AbilityInfo.InputTag = MyAbilitySystemComponent->GetInputTagByAbilitySpec(AbilitySpec);
		OnEachAbilityInfoDelegate.Broadcast(AbilityInfo);
	});
	
	MyAbilitySystemComponent->ForEachAbility(AbilityDelegate);
}
