// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GAST_Gamemodebase.generated.h"
class USaveGame;
class ULoadSlotSaveGame;
class UMVVM_LoadSlotViewModel;
class UMyAbilityInfo;
class UCharacterClassInfo;
/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API AGAST_Gamemodebase : public AGameModeBase
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly,Category="CharacterClassInfo")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly,Category="CharacterClassInfo")
	TObjectPtr<UMyAbilityInfo>AbilityInformation;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> SlotSaveGameClass;

	UPROPERTY(EditDefaultsOnly)
	FString DefaultMapName;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DefaultMap;

	UPROPERTY(EditDefaultsOnly)
	FName DefaultsStartTag;

	UPROPERTY(EditDefaultsOnly)
	TMap<FString,TSoftObjectPtr<UWorld>> Maps;
	
	void SaveLoadSlot(UMVVM_LoadSlotViewModel* LoadSlot,int32 LoadSlotIndex);
	ULoadSlotSaveGame* GetSaveDataFromSlot(const FString& LoadSlotName,int32 Index);
	void DeleteSlot(const FString& LoadSlotName,int32 Index);
	void TravelToMap(UMVVM_LoadSlotViewModel* LoadSlot);
	ULoadSlotSaveGame* RetrievelSaveGameData();
	void SaveInGameProgessData(ULoadSlotSaveGame* SaveData);

	void SavedWorldState(UWorld* World);

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

protected:
	virtual void BeginPlay() override;
};
