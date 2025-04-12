// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GAST_AIController.generated.h"
class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API AGAST_AIController : public AAIController
{
	GENERATED_BODY()
public:
	AGAST_AIController();
protected:

	UPROPERTY(VisibleAnywhere,Category="AI")
	TObjectPtr<UBehaviorTreeComponent>BehaviorTree;
};
