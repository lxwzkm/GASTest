// Fill out your copyright notice in the Description page of Project Settings.


#include "GAST_AbilitySystemLibrary.h"

#include "GAST_AbilityType.h"
#include "Data/CharacterClassInfo.h"
#include "Gamemode/GAST_Gamemodebase.h"
#include "Gamemode/GAST_PlayerState.h"
#include "UI/WidgetController/GAST_WidgetControllerBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/GAST_HUD.h"

UOverlayWidgetController* UGAST_AbilitySystemLibrary::GetOverlayWidgetController(UObject* WordContext)
{
	if (APlayerController* PC= UGameplayStatics::GetPlayerController(WordContext,0))
	{
		if (AGAST_HUD* PlayerHUD=Cast<AGAST_HUD>(PC->GetHUD()))
		{
			if (AGAST_PlayerState* PS= PC->GetPlayerState<AGAST_PlayerState>())
			{
				UAttributeSet* AS=PS->GetAttributeSet();
				UAbilitySystemComponent* ASC=PS->GetAbilitySystemComponent();
				const FWidgetControllerParams Params(PC,PS,AS,ASC);
				return PlayerHUD->GetOverlayWidgetController(Params);
			}
			
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UGAST_AbilitySystemLibrary::GetAttributeMenuWidgetController(UObject* WordContext)
{
	if (APlayerController* PC=UGameplayStatics::GetPlayerController(WordContext,0))
	{
		if (AGAST_HUD*PlayerHUD=Cast<AGAST_HUD>(PC->GetHUD()))
		{
			if (AGAST_PlayerState* PS=PC->GetPlayerState<AGAST_PlayerState>())
			{
				UAttributeSet*AS=PS->GetAttributeSet();
				UAbilitySystemComponent*ASC=PS->GetAbilitySystemComponent();
				FWidgetControllerParams Params(PC,PS,AS,ASC);
				return PlayerHUD->GetAttributeMenuWidgetController(Params);
			}
		}
	}
	return nullptr;
}

void UGAST_AbilitySystemLibrary::InitializeDefaultsAttributes(const UObject* WordContext, ECharacterClass
	CharacterClass, float Level, UAbilitySystemComponent* ASC)
{

	AActor* AvatarActor= ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo=GetCharacterClassInfo(WordContext);
	const FCharacterAttribute PrimaryAttribute= CharacterClassInfo->GetCharacterAttribute(CharacterClass);
	
	FGameplayEffectContextHandle PrimaryContextHandle= ASC->MakeEffectContext();
	PrimaryContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryEffectSpecHandle= ASC->MakeOutgoingSpec(PrimaryAttribute.PrimaryAttribute,Level,PrimaryContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryEffectSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryContextHandle=ASC->MakeEffectContext();
	SecondaryContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryEffectSpecHandle= ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttribute,Level,SecondaryContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryEffectSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalContextHandle=ASC->MakeEffectContext();
	VitalContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalEffectSpecHandle= ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttribute,Level,VitalContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalEffectSpecHandle.Data.Get());
}

void UGAST_AbilitySystemLibrary::InitializeDefaultsAbilities(const UObject* WordContext, UAbilitySystemComponent* ASC)
{
	UCharacterClassInfo* ClassInfo= GetCharacterClassInfo(WordContext);
	if (!ClassInfo)return;

	for (auto Ability:ClassInfo->CommonAbility)
	{
		FGameplayAbilitySpec AbilitySpec= FGameplayAbilitySpec(Ability,1);
		ASC->GiveAbility(AbilitySpec);
	}
}

UCharacterClassInfo* UGAST_AbilitySystemLibrary::GetCharacterClassInfo(const UObject* WordContext)
{
	
	AGAST_Gamemodebase*Gamemodebase=Cast<AGAST_Gamemodebase>(UGameplayStatics::GetGameMode(WordContext));
	if (Gamemodebase==nullptr)return nullptr;

	return Gamemodebase->CharacterClassInfo;
}

bool UGAST_AbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& GameplayEffectContextHandle)
{
	const FGameplayEffectContext* EffectContext=GameplayEffectContextHandle.Get();
	const FMyGameplayEffectContext* MyEffectContext=static_cast<const FMyGameplayEffectContext*>(EffectContext);
	if (MyEffectContext)
	{
		return MyEffectContext->IsBlockHit();
	}
	return false;
}

bool UGAST_AbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& GameplayEffectContextHandle)
{
	const FGameplayEffectContext* EffectContext=GameplayEffectContextHandle.Get();
	const FMyGameplayEffectContext* MyEffectContext=static_cast<const FMyGameplayEffectContext*>(EffectContext);
	if (MyEffectContext)
	{
		return MyEffectContext->IsCriticalHit();
	}
	return false;
}

void UGAST_AbilitySystemLibrary::SetIsBlockHit(FGameplayEffectContextHandle& GameplayEffectContextHandle,
	bool bInIsBlocked)
{
	FGameplayEffectContext* EffectContext=GameplayEffectContextHandle.Get();
	FMyGameplayEffectContext* MyEffectContext=static_cast<FMyGameplayEffectContext*>(EffectContext);
	MyEffectContext->SetIsBlockHit(bInIsBlocked);
}

void UGAST_AbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& GameplayEffectContextHandle,
	bool bInIsCritical)
{
	FGameplayEffectContext* EffectContext=GameplayEffectContextHandle.Get();
	FMyGameplayEffectContext* MyEffectContext=static_cast<FMyGameplayEffectContext*>(EffectContext);
	MyEffectContext->SetIsCriticalHit(bInIsCritical);
}
