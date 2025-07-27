// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/PassiveSpell/PassiveSpellNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GAST_AbilitySystemLibrary.h"
#include "AbilitySystem/GAST_AbilitySystemComponent.h"
#include "GameplayTag/GAST_GameplayTags.h"
#include "Interaction/CombatInterface.h"

UPassiveSpellNiagaraComponent::UPassiveSpellNiagaraComponent()
{
	bAutoActivate = false;
}

void UPassiveSpellNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
	//需要判断当前的ASC是否已经存在
	if (UGAST_AbilitySystemComponent* MyASC=Cast<UGAST_AbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
	{
		MyASC->OnActivePassiveNiagaraDelegate.AddUObject(this,& UPassiveSpellNiagaraComponent::ReciveActiveNiagara);
		ActivateIfEquipped(MyASC);
	}
	else if (ICombatInterface* CombatInterface=Cast<ICombatInterface>(GetOwner()))
	{
		//如果不存在，就绑定一个回调函数，等待ASC建立之后，会触发这个lambda函数，来绑定ASC
		CombatInterface->GetOnASCRegistered().AddLambda([this](UAbilitySystemComponent* ASC)
		{
			if (UGAST_AbilitySystemComponent* MyASC=Cast<UGAST_AbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
			{
				MyASC->OnActivePassiveNiagaraDelegate.AddUObject(this,& UPassiveSpellNiagaraComponent::ReciveActiveNiagara);
				ActivateIfEquipped(MyASC);
			}
		});
	}
}

void UPassiveSpellNiagaraComponent::ReciveActiveNiagara(const FGameplayTag& AbilityTag, bool bIsActiveCom)
{
	if (AbilityTag.MatchesTagExact(AbilityPassiveTag))
	{
		if (bIsActiveCom)
		{
			Activate(true);
		}
		else
		{
			Deactivate();
		}
	}
	
}

void UPassiveSpellNiagaraComponent::ActivateIfEquipped(UGAST_AbilitySystemComponent* MyASC)
{
	if (MyASC->bGivenAbility)
	{
		if (MyASC->GetStatusByAbiltyTag(AbilityPassiveTag).MatchesTagExact(FGameplayTags::Get().Ability_Status_Equipped))
		{
			Activate();
		}
	}
}
