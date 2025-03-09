// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GAST_PlayerCOntroller.generated.h"

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
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;//这个是绑定输入操作的函数
private:
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputMappingContext> PlayerContext;

	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputAction>MoveAction;

	void Move(const FInputActionValue& InputActionValue);//移动操作的回调函数
};
