// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadSlotViewModel.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSetSwitcherIndex,int32,SwitcherIndex);

/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API UMVVM_LoadSlotViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintAssignable)
	FOnSetSwitcherIndex SetSwitcherIndex;

	void SetLoadSlotName(const FString& NewLoadSlotName);
	void SetPlayerName(const FString& NewPlayerName);
	
	FString GetLoadSlotName()const { return LoadSlotName; };
	FString GetPlayerName()const { return PlayerName; };
	void InitializeSlot();


private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess="true"));
	FString LoadSlotName;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess="true"))
	FString PlayerName;
};
