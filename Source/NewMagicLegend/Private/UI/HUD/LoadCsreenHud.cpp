// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/LoadCsreenHud.h"

#include "Blueprint/UserWidget.h"
#include "UI/MVVM/MVVM_LoadScreenViewModel.h"
#include "UI/Widget/LoadScreenWidget.h"

void ALoadCsreenHud::BeginPlay()
{
	Super::BeginPlay();

	LoadScreenViewModel=NewObject<UMVVM_LoadScreenViewModel>(this,LoadScreenViewModeClass);

	LoadScreenWidget=CreateWidget<ULoadScreenWidget>(GetWorld(),LoadCsreenWidgetClass);
	LoadScreenWidget->AddToViewport();
}
