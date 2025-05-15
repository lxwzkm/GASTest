// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "DebuffNiagaraComponent.generated.h"

/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API UDebuffNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()
public:
	UDebuffNiagaraComponent();

	UPROPERTY(VisibleAnywhere)
	FGameplayTag DebuffTag;
	
	virtual void BeginPlay() override;
	void OnDebuffChanged(const FGameplayTag InDebuffTag,int32 NewCount);
	UFUNCTION()
	void DeathActor(AActor* DeadActor);
};
