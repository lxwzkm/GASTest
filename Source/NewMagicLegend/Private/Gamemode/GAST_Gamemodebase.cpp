// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemode/GAST_Gamemodebase.h"

#include "Gamemode/LoadSlotSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/MVVM/MVVM_LoadSlotViewModel.h"

void AGAST_Gamemodebase::SaveLoadSlot(UMVVM_LoadSlotViewModel* LoadSlot, int32 LoadSlotIndex)
{
	DeleteSlot(LoadSlot->GetLoadSlotName(), LoadSlotIndex);
	
	USaveGame* SaveGameObj= UGameplayStatics::CreateSaveGameObject(SlotSaveGameClass);
	ULoadSlotSaveGame* LoadSlotSaveGame=Cast<ULoadSlotSaveGame>(SaveGameObj);
	LoadSlotSaveGame->PlayerName=LoadSlot->GetPlayerName();
	LoadSlotSaveGame->SlotStatus=LoadSlot->SaveSlotStatus;
	LoadSlotSaveGame->MapName=LoadSlot->GetMapName();

	UGameplayStatics::SaveGameToSlot(LoadSlotSaveGame,LoadSlot->GetLoadSlotName(),LoadSlotIndex);
}

ULoadSlotSaveGame* AGAST_Gamemodebase::GetSaveDataFromSlot(const FString& LoadSlotName, int32 Index)
{
	USaveGame* SaveGameObject=nullptr;
	if (UGameplayStatics::DoesSaveGameExist(LoadSlotName,Index))
	{
		SaveGameObject=UGameplayStatics::LoadGameFromSlot(LoadSlotName,Index);
	}
	else
	{
		SaveGameObject=UGameplayStatics::CreateSaveGameObject(SlotSaveGameClass);
	}
	ULoadSlotSaveGame* LoadSlotViewModel=Cast<ULoadSlotSaveGame>(SaveGameObject);
	return LoadSlotViewModel;
}

void AGAST_Gamemodebase::DeleteSlot(const FString& LoadSlotName, int32 Index)
{
	const bool bLoadSlotExist=UGameplayStatics::DoesSaveGameExist(LoadSlotName,Index);
	if(bLoadSlotExist)
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlotName,Index);
	}
}

void AGAST_Gamemodebase::TravelToMap(UMVVM_LoadSlotViewModel* LoadSlot)
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(LoadSlot,Maps.FindChecked(LoadSlot->GetMapName()));
}

void AGAST_Gamemodebase::BeginPlay()
{
	Super::BeginPlay();

	Maps.Add(DefaultMapName,DefaultMap);
}
