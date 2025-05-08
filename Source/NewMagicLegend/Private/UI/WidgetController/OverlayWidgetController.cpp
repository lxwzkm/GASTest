// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/GAST_AttributeSet.h"
#include "Data/LevelUpInfo.h"
#include "Gamemode/GAST_PlayerState.h"

void UOverlayWidgetController::BroadcastInitValues()
{
	OnHealthChanged.Broadcast(GetAttributeSet()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetAttributeSet()->GetMaxHealth());
	OnManaChanged.Broadcast(GetAttributeSet()->GetMana());
	OnMaxManaChanged.Broadcast(GetAttributeSet()->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	
	/*---------------------------  属性变化绑定  ----------------------------*/
	ASC->GetGameplayAttributeValueChangeDelegate(GetAttributeSet()->GetHealthAttribute()).AddLambda(
   [this](const FOnAttributeChangeData& Data)
   {
   	    OnHealthChanged.Broadcast(Data.NewValue);
   }
	);
	
	ASC->GetGameplayAttributeValueChangeDelegate(GetAttributeSet()->GetMaxHealthAttribute()).AddLambda(
    [this](const FOnAttributeChangeData& Data)
    {
    	OnMaxHealthChanged.Broadcast(Data.NewValue);
    }
	);
	
	ASC->GetGameplayAttributeValueChangeDelegate(GetAttributeSet()->GetManaAttribute()).AddLambda(
    [this](const FOnAttributeChangeData& Data)
    {
    	OnManaChanged.Broadcast(Data.NewValue);
    }
	);
	
	ASC->GetGameplayAttributeValueChangeDelegate(GetAttributeSet()->GetMaxManaAttribute()).AddLambda(
    [this](const FOnAttributeChangeData& Data)
    {
    	OnMaxManaChanged.Broadcast(Data.NewValue); 
    }
	);
	
	GetPlayerState()->OnXPChangeDelegate.AddUObject(this,&UOverlayWidgetController::OnXPChange);
	
	/*---------------------------  发消息给UI  ----------------------------*/
	if (GetAbilitySystemComponent())
	{
		if (GetAbilitySystemComponent()->bGivenAbility)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			GetAbilitySystemComponent()->OnStartupAbilitiesGivenDelegate.AddUObject(this,& UOverlayWidgetController::BroadcastAbilityInfo);
		}
		
		GetAbilitySystemComponent()->AllAssetTagsContainerDelegate.AddLambda(
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
	/*---------------------------  PlayerStateChange  ----------------------------*/
	GetPlayerState()->OnLevelChangeDelegate.AddLambda(
[this](int32 NewLevel)
		{
			OnPlayerLevelChangeDelegate.Broadcast(NewLevel);
		}
	);

}

void UOverlayWidgetController::OnXPChange(int32 XP)
{
	
	ULevelUpInfo* LevelUpInfo=GetPlayerState()->LevelUpInformation;
	check(LevelUpInfo);
	int32 Level=LevelUpInfo->FindLevelForXP(XP);
	int32 MaxLevel=LevelUpInfo->LevelUpInfos.Num();
	if (Level<=MaxLevel&&Level>0)
	{
		//经验是累计计算的，但是进度条只关心当前等级内的进度填充，当前等级升到下一级的经验-前一级升到本级的经验，就是本级升级所需的经验
		int32 LevelRequirement=LevelUpInfo->LevelUpInfos[Level].LevelUpRequirement;
		int32 PreviousLevelRequirement=LevelUpInfo->LevelUpInfos[Level-1].LevelUpRequirement;

		int32 DeltaRequirement=LevelRequirement-PreviousLevelRequirement;

		int32 RealXP=XP-PreviousLevelRequirement;
		float XPPercent=static_cast<float>(RealXP)/static_cast<float>(DeltaRequirement);
		OnXPChangeDelegate.Broadcast(XPPercent);
	}
}
