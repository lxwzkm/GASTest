// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemode/LoadSlotSaveGame.h"

FSavedMap ULoadSlotSaveGame::GetSavedMapByMapName(const FString& InMapName)
{
	for (const FSavedMap& Map:SavedMaps)
	{
		if (Map.MapAssestName==InMapName)
		{
			return Map;
		}
	}
	return FSavedMap();
}

bool ULoadSlotSaveGame::HasMap(const FString& InMapName) const
{
	for (const FSavedMap& Map:SavedMaps)
	{
		if (Map.MapAssestName==InMapName)
		{
			return true;
		}
	}
	return false;
}
