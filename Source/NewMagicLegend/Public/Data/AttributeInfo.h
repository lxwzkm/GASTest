// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AttributeInfo.generated.h"

//该结构体存储了需要WidgetController广播给Widget的一般数据
USTRUCT(BlueprintType)
struct FGAST_AttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag AttributeTag=FGameplayTag();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FText AttributeName=FText();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FText AttributeDescription=FText();

	UPROPERTY(BlueprintReadOnly)
	float AttributeValue=0.f;
};

/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()
public:

	FGAST_AttributeInfo GetAttributeInfoByTag(const FGameplayTag& AttributeTag,bool bLogNotFound=false);
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FGAST_AttributeInfo> AttributeInformation;
};
