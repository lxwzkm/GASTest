// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GAST_FloatTextWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API UGAST_FloatTextWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void SetDamageText(float Damge);
};
