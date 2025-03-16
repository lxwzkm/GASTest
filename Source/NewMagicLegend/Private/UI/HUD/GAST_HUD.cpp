// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/GAST_HUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AGAST_HUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController==nullptr)
	{
		OverlayWidgetController=NewObject<UOverlayWidgetController>(this,OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

void AGAST_HUD::InitOverlayControllerParams(APlayerController* InPC, APlayerState* InPS, UAttributeSet* InAS,
	UAbilitySystemComponent* InASC)
{//1 获取OverlayWidget  2 获取OverlayWidgetController  3 将两者绑定在一起，即SetUserWidgetController  4 通过Controller初始化属性值
	checkf(OverlayWidgetClass,TEXT("OverlayWidgetClass未初始化"));
	checkf(OverlayWidgetControllerClass,TEXT("OverlayWidgetControllerClass未初始化"));
	
	UUserWidget* Widget= CreateWidget<UUserWidget>(GetWorld(),OverlayWidgetClass);
	OverlayWidget=Cast<UGAST_UserWidgetBase>(Widget);

	FWidgetControllerParams WCParams(InPC,InPS,InAS,InASC);
	UOverlayWidgetController*WidgetController= GetOverlayWidgetController(WCParams);

	OverlayWidget->SetUserWidgetController(WidgetController);
	WidgetController->BroadcastInitValues();
	
	OverlayWidget->AddToViewport();
}