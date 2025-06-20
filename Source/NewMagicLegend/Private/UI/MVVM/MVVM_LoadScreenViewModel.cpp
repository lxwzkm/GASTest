// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MVVM/MVVM_LoadScreenViewModel.h"

#include "Gamemode/GAST_Gamemodebase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/MVVM/MVVM_LoadSlotViewModel.h"

void UMVVM_LoadScreenViewModel::InitializeLoadSlot()
{
	LoadSlotViewModel_0=NewObject<UMVVM_LoadSlotViewModel>(this,LoadSlotViewModelClass);
	SlotViewModels.Add(0,LoadSlotViewModel_0);
	LoadSlotViewModel_0->SetLoadSlotName(FString("LoadSlot_0"));
	LoadSlotViewModel_1=NewObject<UMVVM_LoadSlotViewModel>(this,LoadSlotViewModelClass);
	SlotViewModels.Add(1,LoadSlotViewModel_1);
	LoadSlotViewModel_1->SetLoadSlotName(FString("LoadSlot_1"));
	LoadSlotViewModel_2=NewObject<UMVVM_LoadSlotViewModel>(this,LoadSlotViewModelClass);
	SlotViewModels.Add(2,LoadSlotViewModel_2);
	LoadSlotViewModel_2->SetLoadSlotName(FString("LoadSlot_2"));

	SetLoadSlotNum(SlotViewModels.Num());
}

UMVVM_LoadSlotViewModel* UMVVM_LoadScreenViewModel::GetLoadSlotViewModelByIndex(int32 Index)
{
	return SlotViewModels.FindChecked(Index);
}

void UMVVM_LoadScreenViewModel::ButtonNewGamePressed(int32 Slot)
{
	SlotViewModels[Slot]->SetSwitcherIndex.Broadcast(1);
}

void UMVVM_LoadScreenViewModel::ButtonNewSlotPressed(int32 Slot, const FString& SlotName)
{
	if (AGAST_Gamemodebase* MyGameMode=Cast<AGAST_Gamemodebase>(UGameplayStatics::GetGameMode(this)))
	{
		SlotViewModels[Slot]->SetPlayerName(SlotName);
		SlotViewModels[Slot]->InitializeSlot();
		MyGameMode->SaveLoadSlot(SlotViewModels[Slot],Slot);
	}
}

void UMVVM_LoadScreenViewModel::ButtonSelectSlotPressed(int32 Slot)
{
}

void UMVVM_LoadScreenViewModel::SetLoadSlotNum(int32 Num)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotNum, Num);
}