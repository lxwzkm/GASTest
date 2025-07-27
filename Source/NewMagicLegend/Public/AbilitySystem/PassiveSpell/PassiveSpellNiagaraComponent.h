// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "PassiveSpellNiagaraComponent.generated.h"

/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API UPassiveSpellNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()
public:
	UPassiveSpellNiagaraComponent();

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag AbilityPassiveTag;

protected:
	virtual void BeginPlay() override;
	void ReciveActiveNiagara(const FGameplayTag& AbilityTag,bool bIsActiveCom);
	void ActivateIfEquipped(UGAST_AbilitySystemComponent* MyASC);
};
