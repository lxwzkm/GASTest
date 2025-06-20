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

void UMVVM_LoadSlotViewModel::InitializeSlot()
{
	//TODO:检查存档状态，并广播对应的索引坐标
	SetSwitcherIndex.Broadcast(2);
}
