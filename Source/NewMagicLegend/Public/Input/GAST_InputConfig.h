// Fill out your copyright notice in the Description page of Project Settings.
//将技能的输入与Tag进行绑定的配置文件
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputAction.h"
#include "Engine/DataAsset.h"
#include "GAST_InputConfig.generated.h"

USTRUCT(BlueprintType)
struct FMyInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag InputTag=FGameplayTag();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	const UInputAction* AbilityInputAction=nullptr;
};

/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API UGAST_InputConfig : public UDataAsset
{
	GENERATED_BODY()
public:

	const UInputAction* GetInputActionByTag(const FGameplayTag& InputTag,bool bLogNotFound);
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FMyInputAction> AbilityInputActions;
};
