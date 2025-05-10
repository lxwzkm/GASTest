// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/GAST_GameplayAbilityBase.h"

#include "GAST_AbilitySystemLibrary.h"
#include "AbilitySystem/GAST_AttributeSet.h"

FString UGAST_GameplayAbilityBase::GetCurrentLevelDescription(int32 Level)
{
	FString Description=FString::Printf(TEXT("<Default>默认等级描述，默认等级描述，默认等级描述，默认等级描述，默认等级描述，默认等级描述，默认等级描述，默认等级描述，默认等级描述，默认等级描述，默认等级描述默认等级描述</>"));
	return Description;
}

FString UGAST_GameplayAbilityBase::GetNextLevelDescription(int32 Level)
{
	FString Description=FString::Printf(TEXT("<Default>默认等级描述，默认等级描述，默认等级描述，默认等级描述，默认等级描述，默认等级描述，默认等级描述，默认等级描述，默认等级描述，默认等级描述，默认等级描述默认等级描述</>"));
	return Description;
}

FString UGAST_GameplayAbilityBase::GetLockedDescription(int32 Level)
{
	FString Description=FString::Printf(TEXT("<Default>当前技能已锁定</>\n<Default>解锁等级：</> <Level>%d </>"),Level);
	return Description;
}

float UGAST_GameplayAbilityBase::GetManaCost(int32 Level)
{
	float ManaCost=0.0f;
	if (UGameplayEffect* CostEffect=GetCostGameplayEffect())
	{
		for (auto& Modifier:CostEffect->Modifiers)
		{
			if (Modifier.Attribute==UGAST_AttributeSet::GetManaAttribute())
			{
				Modifier.ModifierMagnitude.GetStaticMagnitudeIfPossible(Level,ManaCost);
				break;
			}
		}
	}
	return ManaCost;
}

float UGAST_GameplayAbilityBase::GetCooldown(int32 Level)
{
	float Cooldown=0.0f;
	if (UGameplayEffect* CDEffect=GetCooldownGameplayEffect())
	{
		CDEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(Level,Cooldown);
	}
	return Cooldown;
}

