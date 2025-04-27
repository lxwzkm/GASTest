// Fill out your copyright notice in the Description page of Project Settings.


#include "GAST_AbilitySystemLibrary.h"

#include "GAST_AbilityType.h"
#include "Data/CharacterClassInfo.h"
#include "Gamemode/GAST_Gamemodebase.h"
#include "Gamemode/GAST_PlayerState.h"
#include "Interaction/CombatInterface.h"
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

void UGAST_AbilitySystemLibrary::InitializeDefaultsAbilities(const UObject* WordContext, UAbilitySystemComponent* ASC,ECharacterClass CharacterClass)
{
	UCharacterClassInfo* ClassInfo= GetCharacterClassInfo(WordContext);
	if (!ClassInfo)return;

	for (auto Ability:ClassInfo->CommonAbility)
	{
		FGameplayAbilitySpec AbilitySpec= FGameplayAbilitySpec(Ability,1);
		ASC->GiveAbility(AbilitySpec);
	}

	const FCharacterAttribute StartUpAttribute=ClassInfo->GetCharacterAttribute(CharacterClass);
	if (ICombatInterface* CombatInterface= Cast<ICombatInterface>(ASC->GetAvatarActor()))
	{
		for (auto Ability:StartUpAttribute.StartupAbility)
		{
			FGameplayAbilitySpec AbilitySpec= FGameplayAbilitySpec(Ability,CombatInterface->GetPlayerLevel());
			ASC->GiveAbility(AbilitySpec);
		}
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

void UGAST_AbilitySystemLibrary::GetLivePlayersWithInRadius(const UObject* WordContext,
	TArray<AActor*>& OutOverlapActors, const TArray<AActor*>& ActorsToIgnore, float Radius,const FVector& SphereOrigin)
{
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	if (UWorld* Word=GEngine->GetWorldFromContextObject(WordContext,EGetWorldErrorMode::LogAndReturnNull))
	{
		Word->OverlapMultiByObjectType(Overlaps,SphereOrigin,FQuat::Identity,FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),FCollisionShape::MakeSphere(Radius),CollisionParams);
		//DrawDebugSphere(Word,SphereOrigin,Radius,10,FColor::Green,false,20);
		//获取OverlapResult之后判断其中的Actor是否具有ICombatInterface和是否存活
		for (const FOverlapResult& Overlap:Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>()&&!ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlapActors.AddUnique(Overlap.GetActor());
			}
		}
	}
}

bool UGAST_AbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool bIsPlayer=FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bIsEnemy=FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	const bool bIsFriend=bIsEnemy || bIsPlayer;
	return !bIsFriend;
}

int32 UGAST_AbilitySystemLibrary::GetXPByClassAndLevel(const UObject* WordContext, ECharacterClass CharacterClass,
	int32 Level)
{
	UCharacterClassInfo* ClassInfo= GetCharacterClassInfo(WordContext);
	if (ClassInfo)
	{
		const FCharacterAttribute Attribute= ClassInfo->GetCharacterAttribute(CharacterClass);
		return Attribute.XPReward.GetValueAtLevel(Level);
	}
	return 0;
}
