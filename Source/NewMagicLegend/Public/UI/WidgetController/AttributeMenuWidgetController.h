// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/GAST_WidgetControllerBase.h"
#include "Data/AttributeInfo.h"
#include "AttributeMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeDataChangedSignature,const FGAST_AttributeInfo&,NewInfo);

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
	FOnAttributeDataChangedSignature AttributeInfoDelegate;
protected:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInformation;
};
