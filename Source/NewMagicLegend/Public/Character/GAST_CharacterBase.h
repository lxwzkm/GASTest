// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GAST_CharacterBase.generated.h"

UCLASS(Abstract)//表明该类是一个抽象类
class NEWMAGICLEGEND_API AGAST_CharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AGAST_CharacterBase();

protected:
	virtual void BeginPlay() override;
};
