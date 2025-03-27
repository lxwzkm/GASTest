// Fill out your copyright notice in the Description page of Project Settings.


#include "GAST_AbilitySystemLibrary.h"

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
