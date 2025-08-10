// Fill out your copyright notice in the Description page of Project Settings.

#include "Data/LootTiers.h"

TArray<FLootItem> ULootTiers::GetLootItems()
{
	TArray<FLootItem> ReturnItems;

	for (auto& LootItem : LootItems)
	{
		for (int32 i=0;i<LootItem.MaxSpawnNum;++i)
		{
			if (FMath::FRandRange(1.f,100.f)<LootItem.SpawnChance)
			{
				FLootItem NewLootItem;
				NewLootItem.ActorClass=LootItem.ActorClass;
				NewLootItem.bLevelOverride=LootItem.bLevelOverride;
				ReturnItems.Add(NewLootItem);
			}
		}
	}
	return ReturnItems;
}
