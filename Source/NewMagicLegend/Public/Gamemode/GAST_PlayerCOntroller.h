// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GAST_PlayerCOntroller.generated.h"

class IEnemyInterface;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API AGAST_PlayerCOntroller : public APlayerController
{
	GENERATED_BODY()
public:
	AGAST_PlayerCOntroller();
protected:
	/* ----系统自带的虚函数重写---- */
	virtual void PlayerTick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;//这个是绑定输入操作的函数
private:
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputMappingContext> PlayerContext;

	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputAction>MoveAction;

	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;

	void Move(const FInputActionValue& InputActionValue);//移动操作的回调函数

	void CursorTrace();//内部给PlayerTick调用的函数，主要用来处理鼠标下面是否是敌人来显示或删除高光
};
