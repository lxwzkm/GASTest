// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/AttributeInfo.h"

FGAST_AttributeInfo UAttributeInfo::GetAttributeInfoByTag(const FGameplayTag& AttributeTag, bool bLogNotFound)
{
	for (auto Info:AttributeInformation)
	{
		if (AttributeTag.MatchesTagExact(Info.AttributeTag))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp,Warning,TEXT("Can not found AttributeInfo by Tag [%s], OnAttributeInfo [%s]"),*AttributeTag.ToString(),*GetNameSafe(this));
	}
	return FGAST_AttributeInfo();
}
