// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MVVM/MVVM_LoadScreenViewModel.h"

#include "UI/MVVM/MVVM_LoadSlotViewModel.h"

void UMVVM_LoadScreenViewModel::InitializeLoadSlot()
{
	LoadSlotViewModel_0=NewObject<UMVVM_LoadSlotViewModel>(this,LoadSlotViewModelClass);
	SlotViewModels.Add(0,LoadSlotViewModel_0);
	LoadSlotViewModel_1=NewObject<UMVVM_LoadSlotViewModel>(this,LoadSlotViewModelClass);
	SlotViewModels.Add(1,LoadSlotViewModel_1);
	LoadSlotViewModel_2=NewObject<UMVVM_LoadSlotViewModel>(this,LoadSlotViewModelClass);
	SlotViewModels.Add(2,LoadSlotViewModel_2);
}

UMVVM_LoadSlotViewModel* UMVVM_LoadScreenViewModel::GetLoadSlotViewModelByIndex(int32 Index)
{
	return SlotViewModels.FindChecked(Index);
}
