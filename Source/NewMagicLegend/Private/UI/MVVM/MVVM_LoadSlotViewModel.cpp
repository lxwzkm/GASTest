// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MVVM/MVVM_LoadSlotViewModel.h"

void UMVVM_LoadSlotViewModel::SetLoadSlotName(const FString& NewLoadSlotName)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotName,NewLoadSlotName);
}

void UMVVM_LoadSlotViewModel::SetPlayerName(const FString& NewPlayerName)
{
	UE_MVVM_SET_PROPERTY_VALUE(PlayerName,NewPlayerName);
}

void UMVVM_LoadSlotViewModel::SetMapName(const FString& NewMapName)
{
	UE_MVVM_SET_PROPERTY_VALUE(MapName,NewMapName);
}

void UMVVM_LoadSlotViewModel::SetPlayerLevel(const int32& NewPlayerLevel)
{
	UE_MVVM_SET_PROPERTY_VALUE(PlayerLevel,NewPlayerLevel);
}

void UMVVM_LoadSlotViewModel::InitializeSlot()
{
	const int32 SlotIndex=SaveSlotStatus.GetValue();
	SetSwitcherIndex.Broadcast(SlotIndex);
}
