// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LoadSlotSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API ULoadSlotSaveGame : public USaveGame
{
	GENERATED_BODY()
public:

	UPROPERTY()
	FString LoadSlotName=FString();

	UPROPERTY()
	int32 LoadSlotIndex=0;

	UPROPERTY()
	FString PlayerName=FString("默认名称");
};
