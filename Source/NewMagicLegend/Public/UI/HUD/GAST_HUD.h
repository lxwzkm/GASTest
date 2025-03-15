// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/Widget/GAST_UserWidgetBase.h"
#include "GAST_HUD.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
struct FWidgetControllerParams;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API AGAST_HUD : public AHUD
{
	GENERATED_BODY()
public:
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);//获取Overlay的控制器如果没有就创建一个
	UFUNCTION()
	void InitOverlayControllerParams(APlayerController*InPC,APlayerState*InPS,UAttributeSet*InAS,UAbilitySystemComponent*InASC);//初始化OverlayController并将OverlayWidget和OverlayController绑定在一起

private:
	UPROPERTY()
	TObjectPtr<UGAST_UserWidgetBase>OverlayWidget;
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController>OverlayWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGAST_UserWidgetBase>OverlayWidgetClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController>OverlayWidgetControllerClass;
	
};
