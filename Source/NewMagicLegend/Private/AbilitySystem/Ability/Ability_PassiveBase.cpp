// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Ability_PassiveBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/GAST_AbilitySystemComponent.h"

void UAbility_PassiveBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UGAST_AbilitySystemComponent* MyASC=Cast<UGAST_AbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		MyASC->DeactivePassiveAbilityDelegate.AddUObject(this,&UAbility_PassiveBase::ReciveDeactive);
	}
}

void UAbility_PassiveBase::ReciveDeactive(const FGameplayTag& AbilityTag)
{
	if (AbilityTags.HasTagExact(AbilityTag))
	{
		EndAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true,true);
	}
}
