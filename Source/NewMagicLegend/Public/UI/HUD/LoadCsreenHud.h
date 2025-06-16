// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LoadCsreenHud.generated.h"

class UMVVM_LoadScreenViewModel;
class ULoadScreenWidget;
/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API ALoadCsreenHud : public AHUD
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LoadCsreenWidgetClass;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULoadScreenWidget> LoadScreenWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadScreenViewModel> LoadScreenViewModeClass;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMVVM_LoadScreenViewModel> LoadScreenViewModel;
protected:
	virtual void BeginPlay() override;
};
