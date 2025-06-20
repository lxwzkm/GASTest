// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemode/GAST_Gamemodebase.h"

#include "Gamemode/LoadSlotSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/MVVM/MVVM_LoadSlotViewModel.h"

void AGAST_Gamemodebase::SaveLoadSlot(UMVVM_LoadSlotViewModel* LoadSlot, int32 LoadSlotIndex)
{
	const bool bLoadSlotExist=UGameplayStatics::DoesSaveGameExist(LoadSlot->GetLoadSlotName(),LoadSlotIndex);
	if(bLoadSlotExist)
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->GetLoadSlotName(),LoadSlotIndex);
	}
	USaveGame* SaveGameObj= UGameplayStatics::CreateSaveGameObject(SlotSaveGameClass);
	ULoadSlotSaveGame* LoadSlotSaveGame=Cast<ULoadSlotSaveGame>(SaveGameObj);
	LoadSlotSaveGame->PlayerName=LoadSlot->GetPlayerName();

	UGameplayStatics::SaveGameToSlot(LoadSlotSaveGame,LoadSlot->GetLoadSlotName(),LoadSlotIndex);
}
