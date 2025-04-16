// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/DamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UDamageGameplayAbility::CauseDamageToTarget(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle= MakeOutgoingGameplayEffectSpec(DamageEffectClass,1.f);
	for (auto& pair:DamageTypes)
	{
		float DamageMagnitude=pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle,pair.Key,DamageMagnitude);
	}
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(),UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

FTagMontage UDamageGameplayAbility::GetRandomTagMontage(const TArray<FTagMontage>& TagMontages)
{
	if (TagMontages.Num()>0)
	{
		const int32 Selection=FMath::RandRange(0,TagMontages.Num()-1);
		return TagMontages[Selection];
	}
	return FTagMontage();
}
