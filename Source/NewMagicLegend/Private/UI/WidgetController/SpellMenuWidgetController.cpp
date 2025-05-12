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

	GetAbilitySystemComponent()->OnEquipAbilityDelegate.AddDynamic(this,&USpellMenuWidgetController::OnAbilityEquiped);
	
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
	if (bWaitForEquipAbility)
	{
		FAAbilityInfo Info=AbilityInformation->GetMyAbilityInfoByAbilityTag(AbilityTag);
		FGameplayTag AbilityType=Info.AbilityType;
		bWaitForEquipAbility=true;
		OnStopWaitForEquipDelegate.Broadcast(AbilityType);
		bWaitForEquipAbility=false;
	}
	
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

void USpellMenuWidgetController::DeSelectGlobe()
{
	if (bWaitForEquipAbility)
	{
		FAAbilityInfo Info=AbilityInformation->GetMyAbilityInfoByAbilityTag(SelectAbility.AbilityTag);
		FGameplayTag AbilityType=Info.AbilityType;
		bWaitForEquipAbility=true;
		OnStopWaitForEquipDelegate.Broadcast(AbilityType);
		bWaitForEquipAbility=false;
	}
	SelectAbility.AbilityTag=FGameplayTags::Get().Ability_None;
	SelectAbility.StatusTag=FGameplayTags::Get().Ability_Status_Locked;
	SelectAbility.AbilityLevel=0;

	OnButtonStateChangedDelegate.Broadcast(false,false,0,FString(),FString());
}

void USpellMenuWidgetController::EquipButtonPressed()
{
	FAAbilityInfo Info=AbilityInformation->GetMyAbilityInfoByAbilityTag(SelectAbility.AbilityTag);
	FGameplayTag AbilityType=Info.AbilityType;
	bWaitForEquipAbility=true;
	OnWaitForEquipDelegate.Broadcast(AbilityType);

	//如果当前选择的能力是已经装备状态，则保存它的InputTag
	//FGameplayTag StatusTag=GetAbilitySystemComponent()->GetStatusByAbiltyTag(SelectAbility.AbilityTag);
	if (SelectAbility.AbilityTag.MatchesTagExact(FGameplayTags::Get().Ability_Status_Equipped))
	{
		SelectSlot=GetAbilitySystemComponent()->GetInputTagByAbilityTag(SelectAbility.AbilityTag);
	}
}

void USpellMenuWidgetController::SpellRowGolbePressed(const FGameplayTag& Slot, const FGameplayTag& AbilityType)
{
	if (!bWaitForEquipAbility)return;
	//判断调用类型，防止将主动技能装载被动插槽内
	FGameplayTag SelectType=AbilityInformation->GetMyAbilityInfoByAbilityTag(SelectAbility.AbilityTag).AbilityType;
	if (!AbilityType.MatchesTagExact(SelectType))return;
	
	//更改技能按键，只在服务器执行，调用ASC来更改ability的slot
	GetAbilitySystemComponent()->Server_EquipAbility(SelectAbility.AbilityTag,Slot);
}

void USpellMenuWidgetController::OnAbilityEquiped(const FGameplayTag& AbilityTag, const FGameplayTag& Status,
	const FGameplayTag& SlotTag, const FGameplayTag& PreviousSlot)
{
	//停止动画
	bWaitForEquipAbility=false;

	const FGameplayTags GameplayTags=FGameplayTags::Get();

	//如果previousslot是有效的，就会广播一个空的给前面的slot
	FAAbilityInfo LastInfo;
	LastInfo.AbilityTag=GameplayTags.Ability_None;
	LastInfo.StatusTag=GameplayTags.Ability_Status_Unlocked;
	LastInfo.InputTag=PreviousSlot;
	OnEachAbilityInfoDelegate.Broadcast(LastInfo);

	//再将现在的广播出去，来设置icon和background
	FAAbilityInfo CurrentInfo=AbilityInformation->GetMyAbilityInfoByAbilityTag(AbilityTag);
	CurrentInfo.StatusTag=Status;
	CurrentInfo.InputTag=SlotTag;
	OnEachAbilityInfoDelegate.Broadcast(CurrentInfo);

	//停止动画
	OnStopWaitForEquipDelegate.Broadcast(CurrentInfo.AbilityType);
	OnAbilityReassignedDelegate.Broadcast(AbilityTag);
	DeSelectGlobe();
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
