// Fill out your copyright notice in the Description page of Project Settings.


#include "Debuff/DebuffNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UDebuffNiagaraComponent::UDebuffNiagaraComponent()
{
	bAutoActivate=false;
	
}

void UDebuffNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	ICombatInterface* CombatInterface=Cast<ICombatInterface>(GetOwner());
	UAbilitySystemComponent* ASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	if (ASC)
	{
		ASC->RegisterGameplayTagEvent(DebuffTag,EGameplayTagEventType::NewOrRemoved).AddUObject(this,&UDebuffNiagaraComponent::OnDebuffChanged);
	}
	else if (CombatInterface)
	{
		//接收广播，当ASC有效时会广播
		CombatInterface->GetOnASCRegistered().AddWeakLambda(GetOwner(),[this](UAbilitySystemComponent* InASC)
		{
			InASC->RegisterGameplayTagEvent(DebuffTag,EGameplayTagEventType::NewOrRemoved).AddUObject(this,&UDebuffNiagaraComponent::OnDebuffChanged);
		});
	}
	if (CombatInterface)
	{
		CombatInterface->GetDeathDelegate().AddDynamic(this,&UDebuffNiagaraComponent::DeathActor);
	}
}

void UDebuffNiagaraComponent::OnDebuffChanged(const FGameplayTag InDebuffTag, int32 NewCount)
{
	
	if (NewCount > 0)
	{
		Activate();
	}
	else
	{
		Deactivate();
	}
}

void UDebuffNiagaraComponent::DeathActor(AActor* DeadActor)
{
	Deactivate();
}
