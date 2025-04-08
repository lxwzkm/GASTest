// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GAST_AbilitySystemGlobals.h"
#include "GAST_AbilityType.h"

FGameplayEffectContext* UGAST_AbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FMyGameplayEffectContext();
}
