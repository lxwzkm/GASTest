// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/GAST_InputConfig.h"

const UInputAction* UGAST_InputConfig::GetInputActionByTag(const FGameplayTag& InputTag,bool bLogNotFound)
{
	for (auto Actions:AbilityInputActions)
	{
		if (InputTag.MatchesTagExact(Actions.InputTag))
		{
			return Actions.AbilityInputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp,Warning,TEXT("can not found Actions by Tag[%s],OnInputConfig[%s]"),*InputTag.ToString(),*GetNameSafe(this));
	}
	return nullptr;
}
