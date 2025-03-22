// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GAST_CharacterBase.h"
#include "MyGAST_Character.generated.h"

class UCameraComponent;
class USpringArmComponent;
/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API AMyGAST_Character : public AGAST_CharacterBase
{
	GENERATED_BODY()
public:
	AMyGAST_Character();
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<USpringArmComponent>SpringArm;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UCameraComponent>PlayerCamera;
protected:
	//在服务器上运行的函数，主要是为了服务器上初始化ASC的Actor信息
	virtual void PossessedBy(AController* NewController) override;
	//在客户端调用的函数，为了在客户端初始化ASC的Actor信息
	virtual void OnRep_PlayerState() override;
private:
	virtual void InitActorInfo() override;
};
