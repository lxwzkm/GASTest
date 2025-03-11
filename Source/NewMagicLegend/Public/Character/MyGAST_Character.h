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
};
