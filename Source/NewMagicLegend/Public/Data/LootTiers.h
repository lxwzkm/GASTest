// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LootTiers.generated.h"

class AGAST_EffectActor;

USTRUCT(BlueprintType)
struct FLootItem
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LootTiers")
	TSubclassOf<AGAST_EffectActor> ActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LootTiers")
	float SpawnChance=0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LootTiers")
	int32 MaxSpawnNum=0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LootTiers")
	bool bLevelOverride=false;
};

/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API ULootTiers : public UDataAsset
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	TArray<FLootItem> GetLootItems();
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FLootItem> LootItems;
};
