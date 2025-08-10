// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Gamemode/LoadSlotSaveGame.h"
#include "MVVM_LoadSlotViewModel.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSetSwitcherIndex,int32,SwitcherIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSetEnableSelectButton,bool,EnableSelectButton);

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

	UPROPERTY(BlueprintAssignable)
	FOnSetEnableSelectButton SetEnableSelectButton;

	void SetLoadSlotName(const FString& NewLoadSlotName);
	void SetPlayerName(const FString& NewPlayerName);
	void SetMapName(const FString& NewMapName);
	void SetPlayerLevel(const int32& NewPlayerLevel);
	
	FString GetLoadSlotName()const { return LoadSlotName; };
	FString GetPlayerName()const { return PlayerName; };
	FString GetMapName()const { return MapName; };
	int32 GetPlayerLevel()const { return PlayerLevel; };
	
	void InitializeSlot();

	UPROPERTY()
	TEnumAsByte<SaveSlotStatus> SaveSlotStatus;
	UPROPERTY()
	int32 LoadSlotIndex;
	UPROPERTY()
	FName PlayerStartTag;
	UPROPERTY()
	FString MapAssetName;
private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess="true"));
	FString LoadSlotName;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess="true"))
	FString PlayerName;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess="true"))
	FString MapName;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Getter,meta=(AllowPrivateAccess="true"))
	int32 PlayerLevel;
};
