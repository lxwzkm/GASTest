// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "GAST_AbilitySystemLibrary.h"
#include "AbilitySystem/GAST_AbilitySystemComponent.h"
#include "Data/MyAbilityInfo.h"
#include "Gamemode/GAST_PlayerState.h"
#include "GameplayTag/GAST_GameplayTags.h"

void USpellMenuWidgetController::BroadcastInitValues()
{
	BroadcastAbilityInfo();
	OnSpellPointsChangeDelegate.Broadcast(GetPlayerState()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetAbilitySystemComponent()->OnAbilityStatusChangedDelegate.AddLambda([this](const FGameplayTag& AbilityTag,const FGameplayTag& StatusTag,int32 AbilityLevel)
	{
		if (AbilityTag.MatchesTagExact(SelectAbility.AbilityTag))
		{//判断触发回调的AbilityTag当前所选的AbilityTag，不是的话，不更新SelectAbility的按钮状态
			SelectAbility.StatusTag = StatusTag;
			SelectAbility.AbilityLevel = AbilityLevel;
			bool bEquip = false;
			bool bSpend = false;
			ShouldEnableButton(StatusTag, CurrentSpellPoints, bSpend, bEquip);

			FString Description;
			FString NextLevelDescription;
			GetAbilitySystemComponent()->GetDescriptionByAbilityTag(AbilityTag,Description,NextLevelDescription);
			OnButtonStateChangedDelegate.Broadcast(bSpend, bEquip,AbilityLevel,Description,NextLevelDescription);
		}
		
		if (AbilityInformation)
		{
			FAAbilityInfo Info = AbilityInformation->GetMyAbilityInfoByAbilityTag(AbilityTag);
			Info.StatusTag = StatusTag;
			OnEachAbilityInfoDelegate.Broadcast(Info);
		}
	});

	GetPlayerState()->OnSpellPointsChangeDelegate.AddLambda([this](int32 SpellPoints)
	{
		OnSpellPointsChangeDelegate.Broadcast(SpellPoints);
		
		CurrentSpellPoints = SpellPoints;
		bool bEquip = false;
		bool bSpend = false;
		ShouldEnableButton(SelectAbility.StatusTag, CurrentSpellPoints, bSpend, bEquip);

		FString Description;
		FString NextLevelDescription;
		GetAbilitySystemComponent()->GetDescriptionByAbilityTag(SelectAbility.AbilityTag, Description, NextLevelDescription);
		OnButtonStateChangedDelegate.Broadcast(bSpend, bEquip,SelectAbility.AbilityLevel,Description,NextLevelDescription);
	});
}

void USpellMenuWidgetController::UpdateButtonState(const FGameplayTag& AbilityTag)
{
	const FGameplayTags GameplayTags=FGameplayTags::Get();
	const int32 SpellPoints= GetPlayerState()->GetSpellPoints();
	FGameplayTag StatusTag;

	const bool bTagVaild=AbilityTag.IsValid();
	const bool bTagNone=AbilityTag.MatchesTagExact(GameplayTags.Ability_None);
	FGameplayAbilitySpec*AbilitySpec= GetAbilitySystemComponent()->GetAbilitySpecFromAbilityTag(AbilityTag);
	const bool bSpecVaild= AbilitySpec!=nullptr;
	
	if (!bTagVaild||bTagNone||!bSpecVaild)
	{
		StatusTag=GameplayTags.Ability_Status_Locked;
	}
	else
	{
		StatusTag=GetAbilitySystemComponent()->GetAbilityStatusFromAbilitySpec(*AbilitySpec);
	}
	SelectAbility.AbilityTag=AbilityTag;
	SelectAbility.StatusTag=StatusTag;
	SelectAbility.AbilityLevel=AbilitySpec==nullptr?0:AbilitySpec->Level;
	
	bool bEquip=false;
	bool bSpend=false;
	ShouldEnableButton(StatusTag,SpellPoints,bSpend,bEquip);

	FString Description;
	FString NextLevelDescription;
	GetAbilitySystemComponent()->GetDescriptionByAbilityTag(AbilityTag,Description,NextLevelDescription);
	OnButtonStateChangedDelegate.Broadcast(bSpend,bEquip,SelectAbility.AbilityLevel,Description,NextLevelDescription);
}

void USpellMenuWidgetController::SpendSpellPoints()
{
	if (GetAbilitySystemComponent())
	{
		GetAbilitySystemComponent()->Server_SpelldSpellPoints(SelectAbility.AbilityTag);
	}
}

void USpellMenuWidgetController::ShouldEnableButton(const FGameplayTag& StatusTag, int32 SpellPoints,
	bool& bSpendButtonEnable, bool& bEquipButtonEnable)
{
	bEquipButtonEnable=false;
	bSpendButtonEnable=false;
	const FGameplayTags GameplayTags=FGameplayTags::Get();
	if (StatusTag.MatchesTagExact(GameplayTags.Ability_Status_Equipped))
	{
		bEquipButtonEnable=true;
		if (SpellPoints>0)
		{
			bSpendButtonEnable=true;
		}
	}
	else if (StatusTag.MatchesTagExact(GameplayTags.Ability_Status_Eligible))
	{
		if (SpellPoints>0)
		{
			bSpendButtonEnable=true;
		}
	}
	else if (StatusTag.MatchesTagExact(GameplayTags.Ability_Status_Unlocked))
	{
		bEquipButtonEnable=true;
		if (SpellPoints>0)
		{
			bSpendButtonEnable=true;
		}
	}
}
