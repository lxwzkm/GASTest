// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/GAST_WidgetControllerBase.h"
#include "Data/AttributeInfo.h"
#include "AttributeMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature,const FGAST_AttributeInfo&,Info);

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class NEWMAGICLEGEND_API UAttributeMenuWidgetController : public UGAST_WidgetControllerBase
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable)
	FAttributeInfoSignature AttributeInfoDelegate;

	UPROPERTY(BlueprintAssignable,Category="GAS|PlayerState")
	FOnPlayerStateChangeSignature OnSpellPointsChangeDelegate;

	UPROPERTY(BlueprintAssignable,Category="GAS|PlayerState")
	FOnPlayerStateChangeSignature OnAttributePointsChangeDelegate;

	UFUNCTION(BlueprintCallable)
	void UpgradeAttributePoints(const FGameplayTag& AttributeTag);
protected:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInformation;

private:
	void BroadcastAttributeInfo(const FGameplayAttribute& Attribute,const FGameplayTag& GameplayTag) const;
};
