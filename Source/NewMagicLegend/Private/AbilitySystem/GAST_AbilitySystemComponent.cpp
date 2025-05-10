// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GAST_AbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GAST_AbilitySystemLibrary.h"
#include "GameplayTag/GAST_GameplayTags.h"
#include "AbilitySystem/Ability/GAST_GameplayAbilityBase.h"
#include "Data/MyAbilityInfo.h"
#include "Interaction/PlayerInterface.h"
#include "NewMagicLegend/MyLog.h"

void UGAST_AbilitySystemComponent::AbilitySystemComponentSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UGAST_AbilitySystemComponent::Client_AppliedGameplayEffect);

	
	//GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Cyan,FString::Printf(TEXT("Tag:%s"),*FGameplayTags::Get().Attributes_Secondary_Armor.ToString()));
}

void UGAST_AbilitySystemComponent::Client_AppliedGameplayEffect_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	//首先获取可以广播的Tag，将GE拥有的Tag广播给WidgetController，再让WidgetController广播给HUD
	FGameplayTagContainer TagAssetsContainer;
	EffectSpec.GetAllAssetTags(TagAssetsContainer);
	AllAssetTagsContainerDelegate.Broadcast(TagAssetsContainer);
}

void UGAST_AbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock AbilityListLock(*this);
	for (FGameplayAbilitySpec AbilitySpec:GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(MyLog,Error,TEXT("Delegate Is Not Bound in %hs"),__FUNCTION__);
		}
	}
	
}

FGameplayTag UGAST_AbilitySystemComponent::GetGameplayTagByAbilitySpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (FGameplayTag Tag:AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag("Ability")))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UGAST_AbilitySystemComponent::GetInputTagByAbilitySpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (!AbilitySpec.DynamicAbilityTags.IsEmpty())
	{
		for (FGameplayTag Tag:AbilitySpec.DynamicAbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag("Input")))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UGAST_AbilitySystemComponent::GetAbilityStatusFromAbilitySpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (!AbilitySpec.DynamicAbilityTags.IsEmpty())
	{
		for (FGameplayTag StatusTag:AbilitySpec.DynamicAbilityTags)
		{
			if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag("Ability.Status")))
			{
				return StatusTag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayAbilitySpec* UGAST_AbilitySystemComponent::GetAbilitySpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	for (auto& AbilitySpec:GetActivatableAbilities())
	{
		for (FGameplayTag Tag:AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTagExact(AbilityTag))
				return &AbilitySpec;
		}
	}
	return nullptr;
}

void UGAST_AbilitySystemComponent::GiveCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbility)
{
	for (auto Ability:StartupAbility)
	{
		FGameplayAbilitySpec AbilitySpec= FGameplayAbilitySpec(Ability,1);
		//Cast不能从TSubClassOf直接转换成UGAST_GameplayAbilityBase，所以需要先通过Class来实例化一个Spec
		if (const UGAST_GameplayAbilityBase* PlayerAbility=Cast<UGAST_GameplayAbilityBase>(AbilitySpec.Ability))
		{//将初始能力中的输入Tag与AbilitySpec绑定，并提交，后续激活时需要对比输入Tag
			AbilitySpec.DynamicAbilityTags.AddTag(PlayerAbility->StartupInputTag);
			//将技能状态添加上去
			AbilitySpec.DynamicAbilityTags.AddTag(FGameplayTags::Get().Ability_Status_Equipped);
			GiveAbility(AbilitySpec);//需要使用Spec
		}
		bGivenAbility=true;
		OnStartupAbilitiesGivenDelegate.Broadcast();
	}
}

void UGAST_AbilitySystemComponent::GiveCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbility)
{
	FScopedAbilityListLock AbilityListLock(*this);
	for (auto AbilitySpec:StartupPassiveAbility)
	{
		FGameplayAbilitySpec Ability= FGameplayAbilitySpec(AbilitySpec,1);
		GiveAbilityAndActivateOnce(Ability);
	}
}

void UGAST_AbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	if (!bGivenAbility)//因为这个变量不是可复制的，所以在客户端就是false
	{
		//广播一次之后，将其设置为true，防止多次进行广播
		bGivenAbility=true;
		OnStartupAbilitiesGivenDelegate.Broadcast();
	}
}

void UGAST_AbilitySystemComponent::AbilityInputHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())return;

	for (auto& AbilitySpec:GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UGAST_AbilitySystemComponent::AbilityInputReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())return;

	for (auto& AbilitySpec:GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UGAST_AbilitySystemComponent::UpdateAbilityStatus(int32 Level)
{
	UMyAbilityInfo* AbilityInfo=UGAST_AbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	for (auto Info:AbilityInfo->AbilityInformation)
	{
		if (Level<Info.LevelUpRequirement)continue;
		if (!Info.AbilityTag.IsValid())continue;

		if (GetAbilitySpecFromAbilityTag(Info.AbilityTag)==nullptr)
		{
			FGameplayAbilitySpec AbilitySpec=FGameplayAbilitySpec(Info.AbilityClass,1);
			AbilitySpec.DynamicAbilityTags.AddTag(FGameplayTags::Get().Ability_Status_Eligible);
			GiveAbility(AbilitySpec);
			
			//使其立刻Replicate
			MarkAbilitySpecDirty(AbilitySpec);
			Client_ChangeAbilityStatus(Info.AbilityTag,FGameplayTags::Get().Ability_Status_Eligible,1);
		}
	}
}

void UGAST_AbilitySystemComponent::Client_ChangeAbilityStatus_Implementation(const FGameplayTag& AbilityTag,const FGameplayTag& StatusTag,int32 AbilityLevel)
{
	OnAbilityStatusChangedDelegate.Broadcast(AbilityTag,StatusTag,AbilityLevel);
}

void UGAST_AbilitySystemComponent::UpgradeAttributePoints(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if (IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor())>0)
		{
			Server_UpgradeAttributePoints(AttributeTag);
		}
	}
}

void UGAST_AbilitySystemComponent::Server_UpgradeAttributePoints_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData EventData;
	EventData.EventTag = AttributeTag;
	EventData.EventMagnitude=1.f;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(),AttributeTag,EventData);

	IPlayerInterface::Execute_AddToAttributePoints(GetAvatarActor(),-1);
}

void UGAST_AbilitySystemComponent::Server_SpelldSpellPoints_Implementation(const FGameplayTag& AbilityTag)
{
	if (FGameplayAbilitySpec* AbilitySpec=GetAbilitySpecFromAbilityTag(AbilityTag))
	{
		FGameplayTags GameplayTags=FGameplayTags::Get();
		if (GetAvatarActor()->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_AddToSpellPoints(GetAvatarActor(),-1);
		}
	
		FGameplayTag Status= GetAbilityStatusFromAbilitySpec(*AbilitySpec);
		if (Status.MatchesTagExact(GameplayTags.Ability_Status_Eligible))
		{
			//技能解锁
			AbilitySpec->DynamicAbilityTags.RemoveTag(GameplayTags.Ability_Status_Eligible);
			AbilitySpec->DynamicAbilityTags.AddTag(GameplayTags.Ability_Status_Unlocked);
			Status=GameplayTags.Ability_Status_Unlocked;
		}
		else if (Status.MatchesTagExact(GameplayTags.Ability_Status_Equipped)||Status.MatchesTagExact(GameplayTags.Ability_Status_Unlocked))
		{
			//技能升级
			AbilitySpec->Level++;
			
		}
		Client_ChangeAbilityStatus(AbilityTag,Status,AbilitySpec->Level);
		MarkAbilitySpecDirty(*AbilitySpec);
	}
}

bool UGAST_AbilitySystemComponent::GetDescriptionByAbilityTag(const FGameplayTag& AbilityTag,FString& OutDescription, FString& OutNextDescription)
{
	if (FGameplayAbilitySpec* AbilitySpec=GetAbilitySpecFromAbilityTag(AbilityTag))
	{
		UGAST_GameplayAbilityBase* AbilityBase=Cast<UGAST_GameplayAbilityBase>(AbilitySpec->Ability);
		OutDescription=AbilityBase->GetCurrentLevelDescription(AbilitySpec->Level);
		OutNextDescription=AbilityBase->GetNextLevelDescription(AbilitySpec->Level+1);
		return true;
	}
	
	UMyAbilityInfo* AbilityInfo = UGAST_AbilitySystemLibrary::GetAbilityInfoFromPlayerState(GetAvatarActor());
	if (!AbilityTag.IsValid() || AbilityTag.MatchesTagExact(FGameplayTags::Get().Ability_None))
	{
		OutDescription = FString();
	}
	else
	{
		OutDescription = UGAST_GameplayAbilityBase::GetLockedDescription(AbilityInfo->GetMyAbilityInfoByAbilityTag(AbilityTag).LevelUpRequirement);
	}
	OutNextDescription = FString();
	return false;
}
