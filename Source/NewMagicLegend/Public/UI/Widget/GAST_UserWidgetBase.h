// Fill out your copyright notice in the Description page of Project Settings.
/*
 * MVC框架，模型数据（model）、视图（view）、控制器（controller），依赖方向是view->Controller->model,Controller从model中获取数据，广播到view显示
 */
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "GAST_UserWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API UGAST_UserWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	void SetUserWidgetController(UObject* InController);//设置Controller
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject>WidgetController;
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void UserwidgetControllerSet();//在蓝图中实现当Controller被设置之后应该实现的功能
};
