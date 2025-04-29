// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class NEWMAGICLEGEND_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetAttributePointsReward(int32 Level);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetSpellPointsReward(int32 Level);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddToAttributePoints(int32 InAttributePoints);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddToSpellPoints(int32 InSpellPoints);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddToLevel(int32 InLevel);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 FindLevelForXP(int32 InXP);
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetXP();
	
	UFUNCTION(BlueprintNativeEvent)
	void AddToXP(int32 InXP);

	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();
};
