// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SavedInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USavedInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class NEWMAGICLEGEND_API ISavedInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool ShouldReplaceTransform();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void LoadActor();
};
