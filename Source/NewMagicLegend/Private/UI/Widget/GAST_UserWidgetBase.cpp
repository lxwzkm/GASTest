// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/GAST_UserWidgetBase.h"

void UGAST_UserWidgetBase::SetUserWidgetController(UObject* InController)
{
	WidgetController=InController;
	UserwidgetControllerSet();//控制器已经设置
}
