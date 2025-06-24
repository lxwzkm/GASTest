// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MVVM/MVVM_LoadScreenViewModel.h"

#include "Gamemode/GAST_GameInstance.h"
#include "Gamemode/GAST_Gamemodebase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/MVVM/MVVM_LoadSlotViewModel.h"

void UMVVM_LoadScreenViewModel::InitializeLoadSlot()
{
	LoadSlotViewModel_0=NewObject<UMVVM_LoadSlotViewModel>(this,LoadSlotViewModelClass);
	SlotViewModels.Add(0,LoadSlotViewModel_0);
	LoadSlotViewModel_0->SetLoadSlotName(FString("LoadSlot_0"));
	LoadSlotViewModel_0->LoadSlotIndex=0;
	LoadSlotViewModel_1=NewObject<UMVVM_LoadSlotViewModel>(this,LoadSlotViewModelClass);
	SlotViewModels.Add(1,LoadSlotViewModel_1);
	LoadSlotViewModel_1->SetLoadSlotName(FString("LoadSlot_1"));
	LoadSlotViewModel_1->LoadSlotIndex=1;
	LoadSlotViewModel_2=NewObject<UMVVM_LoadSlotViewModel>(this,LoadSlotViewModelClass);
	SlotViewModels.Add(2,LoadSlotViewModel_2);
	LoadSlotViewModel_2->SetLoadSlotName(FString("LoadSlot_2"));
	LoadSlotViewModel_2->LoadSlotIndex=2;

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
		SlotViewModels[Slot]->SaveSlotStatus=Taken;
		SlotViewModels[Slot]->SetMapName(MyGameMode->DefaultMapName);
		SlotViewModels[Slot]->PlayerStartTag=MyGameMode->DefaultsStartTag;
		
		MyGameMode->SaveLoadSlot(SlotViewModels[Slot],Slot);
		SlotViewModels[Slot]->InitializeSlot();

		UGAST_GameInstance* MyGameInstance=Cast<UGAST_GameInstance>(MyGameMode->GetGameInstance());
		MyGameInstance->LoadSlotName=SlotName;
		MyGameInstance->LoadSlotIndex=Slot;
		MyGameInstance->PlayerStartTag=MyGameMode->DefaultsStartTag;
	}
}

void UMVVM_LoadScreenViewModel::ButtonSelectSlotPressed(int32 Slot)
{
	OnButtonSelected.Broadcast();
	for (auto Pair:SlotViewModels)
	{
		if (Slot==Pair.Key)
		{
			Pair.Value->SetEnableSelectButton.Broadcast(false);
		}
		else
		{
			Pair.Value->SetEnableSelectButton.Broadcast(true);
		}
	}
	SelectedLoadSlot=SlotViewModels[Slot];
}

void UMVVM_LoadScreenViewModel::DeleteButtonPressed()
{
	if (SelectedLoadSlot)
	{
		AGAST_Gamemodebase* Gamemodebase=Cast<AGAST_Gamemodebase>(UGameplayStatics::GetGameMode(this));
		Gamemodebase->DeleteSlot(SelectedLoadSlot->GetLoadSlotName(),SelectedLoadSlot->LoadSlotIndex);

		SelectedLoadSlot->SaveSlotStatus=Vacant;
		SelectedLoadSlot->InitializeSlot();
		SelectedLoadSlot->SetEnableSelectButton.Broadcast(true);
	}
}

void UMVVM_LoadScreenViewModel::PlayButtonPressed()
{
	AGAST_Gamemodebase* Gamemodebase=Cast<AGAST_Gamemodebase>(UGameplayStatics::GetGameMode(this));
	UGAST_GameInstance* MyGameInstance=Cast<UGAST_GameInstance>(Gamemodebase->GetGameInstance());
	
	if (SelectedLoadSlot)
	{
		MyGameInstance->PlayerStartTag=SelectedLoadSlot->PlayerStartTag;
		Gamemodebase->TravelToMap(SelectedLoadSlot);
	}
}

void UMVVM_LoadScreenViewModel::SetLoadSlotNum(int32 Num)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotNum, Num);
}

void UMVVM_LoadScreenViewModel::LoadData()
{
	AGAST_Gamemodebase* MyGameMode=Cast<AGAST_Gamemodebase>(UGameplayStatics::GetGameMode(this));
	for (auto Slot:SlotViewModels)
	{
		ULoadSlotSaveGame* SaveSlotObject=MyGameMode->GetSaveDataFromSlot(Slot.Value->GetLoadSlotName(),Slot.Key);

		Slot.Value->SaveSlotStatus=SaveSlotObject->SlotStatus;
		Slot.Value->SetPlayerName(SaveSlotObject->PlayerName);
		Slot.Value->SetMapName(SaveSlotObject->MapName);
		Slot.Value->PlayerStartTag=SaveSlotObject->PlayerStartTag;
		Slot.Value->InitializeSlot();
	}
}