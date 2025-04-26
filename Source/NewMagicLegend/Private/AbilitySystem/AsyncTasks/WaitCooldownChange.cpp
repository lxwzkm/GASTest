// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AsyncTasks/WaitCooldownChange.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"

UWaitCooldownChange* UWaitCooldownChange::WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent,const FGameplayTag& InCooldownTag)
{
	UWaitCooldownChange* WaitCooldownChange=NewObject<UWaitCooldownChange>();
	WaitCooldownChange->ASC=AbilitySystemComponent;
	WaitCooldownChange->CooldownTag=InCooldownTag;

	if (!IsValid(AbilitySystemComponent)||!InCooldownTag.IsValid())
	{
		WaitCooldownChange->EndTask();
		return nullptr;
	}

	AbilitySystemComponent->RegisterGameplayTagEvent(InCooldownTag,EGameplayTagEventType::NewOrRemoved).AddUObject(WaitCooldownChange,&UWaitCooldownChange::CooldownTagChanged);
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCooldownChange,&UWaitCooldownChange::OnActiveEffectAdded);

	return WaitCooldownChange;
}

void UWaitCooldownChange::EndTask()
{
	if (!IsValid(ASC))return;
	ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);

	SetReadyToDestroy();
	MarkAsGarbage();
}

void UWaitCooldownChange::CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if (NewCount==0)
	{
		CooldownEnd.Broadcast(0.f);
	}
}

void UWaitCooldownChange::OnActiveEffectAdded(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle GameplayEffectHandle)
{
	FGameplayTagContainer AssetTags;
	GameplayEffectSpec.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantTags;
	GameplayEffectSpec.GetAllGrantedTags(GrantTags);

	if (AssetTags.HasTagExact(CooldownTag)||GrantTags.HasTagExact(CooldownTag))
	{
		FGameplayEffectQuery Query=FGameplayEffectQuery::MakeQuery_MatchAllOwningTags(CooldownTag.GetSingleTagContainer());
		TArray<float> RemainingTimes= AbilitySystemComponent->GetActiveEffectsTimeRemaining(Query);
		if (RemainingTimes.Num()>0)
		{
			float RemainingTime=RemainingTimes[0];
			for (float Time:RemainingTimes)
			{
				if (Time>RemainingTime)
				{
					RemainingTime=Time;
				}
			}
			CooldownStart.Broadcast(RemainingTime);
		}
	}
}
