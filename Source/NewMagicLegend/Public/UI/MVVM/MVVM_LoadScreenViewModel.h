// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreenViewModel.generated.h"

class UMVVM_LoadSlotViewModel;
/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API UMVVM_LoadScreenViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:

	void InitializeLoadSlot();

	UFUNCTION(BlueprintPure)
	UMVVM_LoadSlotViewModel* GetLoadSlotViewModelByIndex(int32 Index);
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSlotViewModel> LoadSlotViewModelClass;
private:
	UPROPERTY()
	TMap<int32,UMVVM_LoadSlotViewModel*> SlotViewModels;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlotViewModel> LoadSlotViewModel_0;
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlotViewModel> LoadSlotViewModel_1;
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlotViewModel> LoadSlotViewModel_2;
};
