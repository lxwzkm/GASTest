// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GAST_AbilitySystemComponent.h"

#include <filesystem>

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

FGameplayTag UGAST_AbilitySystemComponent::GetAbilityTagByAbilitySpec(const FGameplayAbilitySpec& AbilitySpec)
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

FGameplayTag UGAST_AbilitySystemComponent::GetInputTagByAbilityTag(const FGameplayTag& AbilityTag)
{
	FGameplayTag InputTag=FGameplayTag();
	if (FGameplayAbilitySpec* AbilitySpec=GetAbilitySpecFromAbilityTag(AbilityTag))
	{
		InputTag = GetInputTagByAbilitySpec(*AbilitySpec);
	}
	return InputTag;
}

FGameplayTag UGAST_AbilitySystemComponent::GetStatusByAbiltyTag(const FGameplayTag& AbilityTag)
{
	FGameplayTag Status=FGameplayTag();
	if (FGameplayAbilitySpec* AbilitySpec=GetAbilitySpecFromAbilityTag(AbilityTag))
	{
		Status = GetAbilityStatusFromAbilitySpec(*AbilitySpec);
	}
	return Status;
}

FGameplayAbilitySpec* UGAST_AbilitySystemComponent::GetAbilitySpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ScopedLocked(*this);
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

bool UGAST_AbilitySystemComponent::IsSlotEmpty(const FGameplayTag& Slot)
{
	FScopedAbilityListLock AbilityListLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec:GetActivatableAbilities())
	{
		if (IsAbilityHasSlot(AbilitySpec, Slot))
		{
			return false;
		}
	}
	return true;
}

bool UGAST_AbilitySystemComponent::IsAbilityHasSlot(const FGameplayAbilitySpec& AbilitySpec,
	const FGameplayTag& SlotTag)
{
	return AbilitySpec.DynamicAbilityTags.HasTagExact(SlotTag);
}

bool UGAST_AbilitySystemComponent::IsPassiveAbility(const FGameplayAbilitySpec& AbilitySpec)
{
	UMyAbilityInfo* AbilityInfo=UGAST_AbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	const FGameplayTag AbilityTag=GetAbilityTagByAbilitySpec(AbilitySpec);
	const FAAbilityInfo Info=AbilityInfo->GetMyAbilityInfoByAbilityTag(AbilityTag);
	return Info.AbilityType.MatchesTagExact(FGameplayTags::Get().Ability_Type_Passive);
}

FGameplayAbilitySpec* UGAST_AbilitySystemComponent::GetSlotAbility(const FGameplayTag& Slot)
{
	FScopedAbilityListLock AbilityListLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec:GetActivatableAbilities())
	{
		if (IsAbilityHasSlot(AbilitySpec, Slot))
		{
			return &AbilitySpec;
		}
	}
	return nullptr;
}

bool UGAST_AbilitySystemComponent::IsAbilityAlreadyEquipped(const FGameplayAbilitySpec& AbilitySpec)
{
	return AbilitySpec.DynamicAbilityTags.HasTag(FGameplayTag::RequestGameplayTag("Input"));
}

void UGAST_AbilitySystemComponent::AssignAbilityInSlot(FGameplayAbilitySpec& Spec, const FGameplayTag& Slot)
{
	ClearSlot(&Spec);
	Spec.DynamicAbilityTags.AddTag(Slot);
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

void UGAST_AbilitySystemComponent::AbilityInputPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())return;

	FScopedAbilityListLock ScopedLocked(*this);
	for (auto& AbilitySpec:GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (AbilitySpec.IsActive())
			{
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed,AbilitySpec.Handle,AbilitySpec.ActivationInfo.GetActivationPredictionKey());
			}
		}
	}
}

void UGAST_AbilitySystemComponent::AbilityInputHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())return;

	FScopedAbilityListLock ScopedLocked(*this);
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

	FScopedAbilityListLock ScopedLocked(*this);
	for (auto& AbilitySpec:GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag) && AbilitySpec.IsActive())
		{
			AbilitySpecInputReleased(AbilitySpec);
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased,AbilitySpec.Handle,AbilitySpec.ActivationInfo.GetActivationPredictionKey());
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
		if (IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0)
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

void UGAST_AbilitySystemComponent::Server_EquipAbility_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& SlotTag)
{
	if (FGameplayAbilitySpec* AbilitySpec=GetAbilitySpecFromAbilityTag(AbilityTag))
	{
		
		const FGameplayTag PreviouSlot=GetInputTagByAbilitySpec(*AbilitySpec);
		FGameplayTag Status= GetAbilityStatusFromAbilitySpec(*AbilitySpec);
		
		const bool bStatusVaild= Status==FGameplayTags::Get().Ability_Status_Equipped||Status==FGameplayTags::Get().Ability_Status_Unlocked;
		if (bStatusVaild)
		{
			if (!IsSlotEmpty(SlotTag))
			{
				if (FGameplayAbilitySpec* SpecWitSlot=GetSlotAbility(SlotTag))
				{
					//当前要装备的技能槽已经装备了这个技能
					if (AbilitySpec->DynamicAbilityTags.HasTagExact(SlotTag))
					{
						Client_EquipAbility(AbilityTag,SlotTag,FGameplayTags::Get().Ability_Status_Equipped,PreviouSlot);
						return;
					}

					//判断当前要装备的slot内的技能是否为被动技能
					if (IsPassiveAbility(*SpecWitSlot))
					{
						FGameplayTag PreslotAbilityTag=GetAbilityTagByAbilitySpec(*SpecWitSlot);
						//关掉前一个Niagara特效
						Multicast_AcitvePassiveNiagara(PreslotAbilityTag,false);
						DeactivePassiveAbilityDelegate.Broadcast(PreslotAbilityTag);
					}
					//清除原slot内的插槽
					ClearSlot(SpecWitSlot);
				}
			}
			//判断当前技能之前是否是装备过的，来确定是否需要激活被动技能
			if (!IsAbilityAlreadyEquipped(*AbilitySpec))
			{
				//如果没有装备过且为被动技能，激活它，如果是其他情况，被动技能不需要特别处理
				if (IsPassiveAbility(*AbilitySpec))
				{
					//激活Passive的Niagara效果
					Multicast_AcitvePassiveNiagara(AbilityTag,true);
					TryActivateAbility(AbilitySpec->Handle);
				}
			}
			//为装备的技能分配slot
			AssignAbilityInSlot(*AbilitySpec,SlotTag);
			MarkAbilitySpecDirty(*AbilitySpec);
		}
		Client_EquipAbility(AbilityTag,SlotTag,FGameplayTags::Get().Ability_Status_Equipped,PreviouSlot);
	}
}

void UGAST_AbilitySystemComponent::ClearAbilityOfSlot(const FGameplayTag& SlotTag)
{
	FScopedAbilityListLock ScopedLocked(*this);
	for (FGameplayAbilitySpec AbilitySpec:GetActivatableAbilities())
	{
		if (AbilityHasSlot(AbilitySpec, SlotTag))
		{
			ClearSlot(&AbilitySpec);
		}
	}
}

void UGAST_AbilitySystemComponent::ClearSlot( FGameplayAbilitySpec* AbilitySpec)
{
	FGameplayTag SlotTag=GetInputTagByAbilitySpec(*AbilitySpec);
	AbilitySpec->DynamicAbilityTags.RemoveTag(SlotTag);
}

bool UGAST_AbilitySystemComponent::AbilityHasSlot(const FGameplayAbilitySpec& AbilitySpec,
   const FGameplayTag& SlotTag)
{
	for (FGameplayTag Tag:AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTagExact(SlotTag))
		{
			return true;
		}
	}
	return false;
}

void UGAST_AbilitySystemComponent::Client_EquipAbility_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& SlotTag, const FGameplayTag& StautsTag, const FGameplayTag& PreviousSlotTag)
{
	OnEquipAbilityDelegate.Broadcast(AbilityTag,StautsTag,SlotTag,PreviousSlotTag);
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

void UGAST_AbilitySystemComponent::Multicast_AcitvePassiveNiagara_Implementation(const FGameplayTag& AbilityTag,
	bool bIsActiveCom)
{
	OnActivePassiveNiagaraDelegate.Broadcast(AbilityTag, bIsActiveCom);
}
