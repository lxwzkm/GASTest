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

	void SaveLoadSlot(UMVVM_LoadSlotViewModel* LoadSlot,int32 LoadSlotIndex);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> SlotSaveGameClass;
};
