// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTag/GAST_GameplayTags.h"
#include "UI/WidgetController/GAST_WidgetControllerBase.h"
#include "SpellMenuWidgetController.generated.h"

struct FGameplayTags;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnButtonStateChanged, bool, bEnableSpendButton, bool, bEnableEquipButton,int32, AbilityLevel,FString, Description,FString,NextLevelDescription);

struct FSelectAbility
{
	FGameplayTag AbilityTag=FGameplayTag();
	FGameplayTag StatusTag=FGameplayTag();
	int32 AbilityLevel=1;
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class NEWMAGICLEGEND_API USpellMenuWidgetController : public UGAST_WidgetControllerBase
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable,Category="GAS|PlayerState")
	FOnPlayerStateChangeSignature OnSpellPointsChangeDelegate;

	UPROPERTY(BlueprintAssignable,Category="GAS|PlayerState")
	FOnButtonStateChanged OnButtonStateChangedDelegate;

	UFUNCTION(BlueprintCallable)
	void UpdateButtonState(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void SpendSpellPoints();
private:

	void ShouldEnableButton(const FGameplayTag& StatusTag,int32 SpellPoints,bool& bSpendButtonEnable,bool& bEquipButtonEnable);

	FSelectAbility SelectAbility={FGameplayTags::Get().Ability_None,FGameplayTags::Get().Ability_Status_Locked,1};
	int32 CurrentSpellPoints=0;
};
