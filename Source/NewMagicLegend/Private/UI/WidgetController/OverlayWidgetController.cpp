// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/GAST_AttributeSet.h"
#include "Data/LevelUpInfo.h"
#include "Gamemode/GAST_PlayerState.h"

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
	AGAST_PlayerState* PlayerState=Cast<AGAST_PlayerState>(PS);
	check(PlayerState);
	check(AttributeSet);
	
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
	
	PlayerState->OnXPChangeDelegate.AddUObject(this,&UOverlayWidgetController::OnXPChange);
	
	/*---------------------------  发消息给UI  ----------------------------*/
	if (UGAST_AbilitySystemComponent*GAST_ASC=Cast<UGAST_AbilitySystemComponent>(ASC))
	{
		if (GAST_ASC->bGivenAbility)
		{
			OninitializeStartupAbilities(GAST_ASC);
		}
		else
		{
			GAST_ASC->OnStartupAbilitiesGivenDelegate.AddUObject(this,& UOverlayWidgetController::OninitializeStartupAbilities);
		}
		
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

void UOverlayWidgetController::OninitializeStartupAbilities(UGAST_AbilitySystemComponent* AbilitySystemComponent)
{
	//TODO 获取所有起始技能，查找他们所有的信息，广播给Widget
	if (!AbilitySystemComponent->bGivenAbility)return;

	FForEachAbility AbilityDelegate;
	AbilityDelegate.BindLambda([this,AbilitySystemComponent](const FGameplayAbilitySpec& AbilitySpec)
	{
		//TODO 拿到AbilitySpec以后，要获取他们的AbilityInfo
		FGameplayTag AbilityTag = AbilitySystemComponent->GetGameplayTagByAbilitySpec(AbilitySpec);
		FAAbilityInfo AbilityInfo = AbilityInformation->GetMyAbilityInfoByAbilityTag(AbilityTag);
		AbilityInfo.InputTag = AbilitySystemComponent->GetInputTagByAbilitySpec(AbilitySpec);
		OnEachAbilityInfoDelegate.Broadcast(AbilityInfo);

	});
	
	AbilitySystemComponent->ForEachAbility(AbilityDelegate);
}

void UOverlayWidgetController::OnXPChange(int32 XP)
{
	const AGAST_PlayerState* PlayerState=Cast<AGAST_PlayerState>(PS);
	ULevelUpInfo* LevelUpInfo=PlayerState->LevelUpInformation;
	check(LevelUpInfo);
	int32 Level=LevelUpInfo->FindLevelForXP(XP);
	int32 MaxLevel=LevelUpInfo->LevelUpInfos.Num();
	if (Level<=MaxLevel&&Level>0)
	{
		//经验是累计计算的，但是进度条只关心当前等级内的进度填充，前一级的经验可以用来计算当前等级的起始经验值，当前可以升到的等级升级的经验-前一级升级的经验，就是本级升级所需的经验
		int32 LevelRequirement=LevelUpInfo->LevelUpInfos[Level].AttributePointsReward;
		int32 PreviousLevelRequirement=LevelUpInfo->LevelUpInfos[Level-1].AttributePointsReward;

		int32 DeltaRequirement=LevelRequirement-PreviousLevelRequirement;

		int32 RealXP=XP-PreviousLevelRequirement;
		float XPPercent=static_cast<float>(RealXP)/static_cast<float>(DeltaRequirement);
		OnXPChangeDelegate.Broadcast(XPPercent);
	}
}
