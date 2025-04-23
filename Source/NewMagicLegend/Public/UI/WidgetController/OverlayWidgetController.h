// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/MyAbilityInfo.h"
#include "UI/WidgetController/GAST_WidgetControllerBase.h"
#include "OverlayWidgetController.generated.h"


class UGAST_UserWidgetBase;

//这个是作为DT的行结构，为了方便根据Tag来查找数据发送给widget
USTRUCT(BlueprintType)
struct FMessageDataRow:public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGameplayTag MessageTag;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText MessageText;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UGAST_UserWidgetBase> MessageWidget;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UTexture2D* MessageImage;
	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangeSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSendUIMessageSignature,FMessageDataRow,Row);

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class NEWMAGICLEGEND_API UOverlayWidgetController : public UGAST_WidgetControllerBase
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitValues() override;
	virtual void BindCallbacksToDependencies() override;

protected:
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UDataTable>MessageData;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="GAS|Information")
	TObjectPtr<UMyAbilityInfo> AbilityInformation;
	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable,FGameplayTag Tag);
	/*---------------------------  属性变化绑定  ----------------------------*/
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributeChangeSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributeChangeSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributeChangeSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributeChangeSignature OnMaxManaChanged;
	/*---------------------------  发送消息给UI  ----------------------------*/
	UPROPERTY(BlueprintAssignable,Category="GAS|Message")
	FOnSendUIMessageSignature SendUIMessageDelegate;



};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, FGameplayTag Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(),TEXT(""));
}


