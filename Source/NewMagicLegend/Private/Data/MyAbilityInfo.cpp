// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/MyAbilityInfo.h"

#include "NewMagicLegend/MyLog.h"

FAAbilityInfo UMyAbilityInfo::GetMyAbilityInfoByAbilityTag(const FGameplayTag& AbilityTag,bool bNotFound)
{
	for (auto& AbilityInfo:AbilityInformation)
	{
		if (AbilityInfo.AbilityTag==AbilityTag)
		{
			return AbilityInfo;
		}
	}

	if (bNotFound)
	{
		UE_LOG(MyLog, Warning, TEXT("Ability Tag Not Found"));
	}
	
	return FAAbilityInfo();
}
