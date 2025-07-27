// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreenViewModel.generated.h"

class UGameplayAbility;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonSelected);

class UMVVM_LoadSlotViewModel;
/**
 * 
 */

UCLASS()
class NEWMAGICLEGEND_API UMVVM_LoadScreenViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintAssignable)
	FOnButtonSelected OnButtonSelected;
	
	void InitializeLoadSlot();

	UFUNCTION(BlueprintPure)
	UMVVM_LoadSlotViewModel* GetLoadSlotViewModelByIndex(int32 Index);
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSlotViewModel> LoadSlotViewModelClass;

	/*
	 * 不同状态下Slot按钮的回调函数
	 */

	UFUNCTION(BlueprintCallable)
	void ButtonNewGamePressed(int32 Slot);

	UFUNCTION(BlueprintCallable)
	void ButtonNewSlotPressed(int32 Slot,const FString& SlotName);

	UFUNCTION(BlueprintCallable)
	void ButtonSelectSlotPressed(int32 Slot);

	UFUNCTION(BlueprintCallable)
	void DeleteButtonPressed();

	UFUNCTION(BlueprintCallable)
	void PlayButtonPressed();

	int32 GetLoadSlotNum() const { return LoadSlotNum; };
	void SetLoadSlotNum(int32 Num);

	void LoadData();
	
private:
	UPROPERTY()
	TMap<int32,UMVVM_LoadSlotViewModel*> SlotViewModels;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlotViewModel> LoadSlotViewModel_0;
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlotViewModel> LoadSlotViewModel_1;
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlotViewModel> LoadSlotViewModel_2;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlotViewModel> SelectedLoadSlot;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess=true))
	int32 LoadSlotNum;
};
