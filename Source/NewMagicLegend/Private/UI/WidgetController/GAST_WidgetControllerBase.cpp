// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/GAST_WidgetControllerBase.h"

void UGAST_WidgetControllerBase::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PC=WCParams.PlayerController;
	PS=WCParams.PlayerState;
	AS=WCParams.AttributeSet;
	ASC=WCParams.AbilitySystemComponent;
}
