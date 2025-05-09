// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "MyAbilityInfo.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FAAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag AbilityTag=FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag=FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag StatusTag=FGameplayTag();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag CooldownTag=FGameplayTag();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UTexture2D> IconImage=nullptr;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> BackgroundImage=nullptr;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int32 LevelUpRequirement=1;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UGameplayAbility>AbilityClass;
};

/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API UMyAbilityInfo : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FAAbilityInfo>AbilityInformation;

	UFUNCTION(BlueprintCallable)
	FAAbilityInfo GetMyAbilityInfoByAbilityTag(const FGameplayTag& AbilityTag,bool bNotFound=false);
};
