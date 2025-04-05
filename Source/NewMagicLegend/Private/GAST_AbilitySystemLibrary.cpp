// Fill out your copyright notice in the Description page of Project Settings.


#include "GAST_AbilitySystemLibrary.h"

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
	 AGAST_Gamemodebase*Gamemodebase=Cast<AGAST_Gamemodebase>(UGameplayStatics::GetGameMode(WordContext));
	if (Gamemodebase==nullptr)return;

	AActor* AvatarActor= ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo=Gamemodebase->CharacterClassInfo;
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
	AGAST_Gamemodebase*Gamemodebase=Cast<AGAST_Gamemodebase>(UGameplayStatics::GetGameMode(WordContext));
	if (Gamemodebase==nullptr)return;

	for (auto Ability:Gamemodebase->CharacterClassInfo->CommonAbility)
	{
		FGameplayAbilitySpec AbilitySpec= FGameplayAbilitySpec(Ability,1);
		ASC->GiveAbility(AbilitySpec);
	}
}
