// Fill out your copyright notice in the Description page of Project Settings.


#include "GAST_AbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GAST_AbilityType.h"
#include "ImageUtils.h"
#include "Data/CharacterClassInfo.h"
#include "Gamemode/GAST_Gamemodebase.h"
#include "Gamemode/GAST_PlayerState.h"
#include "GameplayTag/GAST_GameplayTags.h"
#include "Interaction/CombatInterface.h"
#include "UI/WidgetController/GAST_WidgetControllerBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/GAST_HUD.h"

bool UGAST_AbilitySystemLibrary::MakeWidgetControllerParams(UObject* WordContext, FWidgetControllerParams& OutWCParams,
	AGAST_HUD*& OutHUD)
{
	if (APlayerController* PC= UGameplayStatics::GetPlayerController(WordContext,0))
	{
		OutHUD=Cast<AGAST_HUD>(PC->GetHUD());
		if (OutHUD)
		{
			if (AGAST_PlayerState* PS= PC->GetPlayerState<AGAST_PlayerState>())
			{
				UAttributeSet* AS=PS->GetAttributeSet();
				UAbilitySystemComponent* ASC=PS->GetAbilitySystemComponent();
				
				OutWCParams.AttributeSet=AS;
				OutWCParams.PlayerController=PC;
				OutWCParams.PlayerState=PS;
				OutWCParams.AbilitySystemComponent=ASC;
				return true;
			}
		}
	}
	return false;
}

UOverlayWidgetController* UGAST_AbilitySystemLibrary::GetOverlayWidgetController(UObject* WordContext)
{
	AGAST_HUD* PlayerHUD=nullptr;
	FWidgetControllerParams WCParams;
	if (MakeWidgetControllerParams(WordContext, WCParams, PlayerHUD))
	{
		return PlayerHUD->GetOverlayWidgetController(WCParams);
	}
	return nullptr;
}

UAttributeMenuWidgetController* UGAST_AbilitySystemLibrary::GetAttributeMenuWidgetController(UObject* WordContext)
{
	AGAST_HUD* PlayerHUD=nullptr;
	FWidgetControllerParams WCParams;
	if (MakeWidgetControllerParams(WordContext, WCParams, PlayerHUD))
	{
		return PlayerHUD->GetAttributeMenuWidgetController(WCParams);
	}
	return nullptr;
}

USpellMenuWidgetController* UGAST_AbilitySystemLibrary::GetSpellMenuWidgetController(UObject* WordContext)
{
	AGAST_HUD* PlayerHUD=nullptr;
	FWidgetControllerParams WCParams;
	if (MakeWidgetControllerParams(WordContext, WCParams, PlayerHUD))
	{
		return PlayerHUD->GetSpellMenuWidgetController(WCParams);
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
	if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
	{
		for (auto Ability:StartUpAttribute.StartupAbility)
		{
			FGameplayAbilitySpec AbilitySpec= FGameplayAbilitySpec(Ability,ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
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

UMyAbilityInfo* UGAST_AbilitySystemLibrary::GetAbilityInfo(const UObject* WordContext)
{
	AGAST_Gamemodebase*Gamemodebase=Cast<AGAST_Gamemodebase>(UGameplayStatics::GetGameMode(WordContext));
	if (Gamemodebase==nullptr)return nullptr;

	return Gamemodebase->AbilityInformation;
}

UMyAbilityInfo* UGAST_AbilitySystemLibrary::GetAbilityInfoFromPlayerState(const UObject* WordContext)
{
	AGAST_PlayerState* PS=Cast<AGAST_PlayerState>(UGameplayStatics::GetPlayerState(WordContext,0));
	if (PS==nullptr)return nullptr;
	return PS->AbilityInformation;
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

bool UGAST_AbilitySystemLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& GameplayEffectContextHandle)
{
	const FGameplayEffectContext* EffectContext=GameplayEffectContextHandle.Get();
	const FMyGameplayEffectContext* MyEffectContext=static_cast<const FMyGameplayEffectContext*>(EffectContext);
	if (MyEffectContext)
	{
		return MyEffectContext->IsSuccessDebuff();
	}
	return false;
}

float UGAST_AbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& GameplayEffectContextHandle)
{
	const FGameplayEffectContext* EffectContext=GameplayEffectContextHandle.Get();
	const FMyGameplayEffectContext* MyEffectContext=static_cast<const FMyGameplayEffectContext*>(EffectContext);
	if (MyEffectContext)
	{
		return MyEffectContext->GetDebuffDamage();
	}
	return 0.f;
}

float UGAST_AbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& GameplayEffectContextHandle)
{
	const FGameplayEffectContext* EffectContext=GameplayEffectContextHandle.Get();
	const FMyGameplayEffectContext* MyEffectContext=static_cast<const FMyGameplayEffectContext*>(EffectContext);
	if (MyEffectContext)
	{
		return MyEffectContext->GetDebuffDuration();
	}
	return 0.f;
}

float UGAST_AbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& GameplayEffectContextHandle)
{
	const FGameplayEffectContext* EffectContext=GameplayEffectContextHandle.Get();
	const FMyGameplayEffectContext* MyEffectContext=static_cast<const FMyGameplayEffectContext*>(EffectContext);
	if (MyEffectContext)
	{
		return MyEffectContext->GetDebuffFrequency();
	}
	return 0.f;
}

FGameplayTag UGAST_AbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& GameplayEffectContextHandle)
{
	const FGameplayEffectContext* EffectContext=GameplayEffectContextHandle.Get();
	const FMyGameplayEffectContext* MyEffectContext=static_cast<const FMyGameplayEffectContext*>(EffectContext);
	if (MyEffectContext)
	{
		if (MyEffectContext->GetDamageType().IsValid())
		{
			return *MyEffectContext->GetDamageType();
		}
		
	}
	return FGameplayTag();
}

FVector UGAST_AbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& GameplayEffectContextHandle)
{
	const FGameplayEffectContext* EffectContext=GameplayEffectContextHandle.Get();
	const FMyGameplayEffectContext* MyEffectContext=static_cast<const FMyGameplayEffectContext*>(EffectContext);
	if (MyEffectContext)
	{
		return MyEffectContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

FVector UGAST_AbilitySystemLibrary::GetKnockBackForce(const FGameplayEffectContextHandle& GameplayEffectContextHandle)
{
	const FGameplayEffectContext* EffectContext=GameplayEffectContextHandle.Get();
	const FMyGameplayEffectContext* MyEffectContext=static_cast<const FMyGameplayEffectContext*>(EffectContext);
	if (MyEffectContext)
	{
		return MyEffectContext->GetKnockBackForce();
	}
	return FVector::ZeroVector;
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

void UGAST_AbilitySystemLibrary::SetIsSuccessfulDebuff(FGameplayEffectContextHandle& GameplayEffectContextHandle,
	bool bInIsSuccessfulDebuff)
{
	FGameplayEffectContext* EffectContext=GameplayEffectContextHandle.Get();
	FMyGameplayEffectContext* MyEffectContext=static_cast<FMyGameplayEffectContext*>(EffectContext);
	MyEffectContext->SetIsSuccessDebuff(bInIsSuccessfulDebuff);
}

void UGAST_AbilitySystemLibrary::SetDebuffDamage(FGameplayEffectContextHandle& GameplayEffectContextHandle,
	float DebuffDamge)
{
	FGameplayEffectContext* EffectContext=GameplayEffectContextHandle.Get();
	FMyGameplayEffectContext* MyEffectContext=static_cast<FMyGameplayEffectContext*>(EffectContext);
	MyEffectContext->SetDebuff_Damage(DebuffDamge);
}

void UGAST_AbilitySystemLibrary::SetDebuffDuration(FGameplayEffectContextHandle& GameplayEffectContextHandle,
	float DebuffDuration)
{
	FGameplayEffectContext* EffectContext=GameplayEffectContextHandle.Get();
	FMyGameplayEffectContext* MyEffectContext=static_cast<FMyGameplayEffectContext*>(EffectContext);
	MyEffectContext->SetDebuff_Duration(DebuffDuration);
}

void UGAST_AbilitySystemLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& GameplayEffectContextHandle,
	float DebuffFrequency)
{
	FGameplayEffectContext* EffectContext=GameplayEffectContextHandle.Get();
	FMyGameplayEffectContext* MyEffectContext=static_cast<FMyGameplayEffectContext*>(EffectContext);
	MyEffectContext->SetDebuff_Frequency(DebuffFrequency);
}

void UGAST_AbilitySystemLibrary::SetDamageType(FGameplayEffectContextHandle& GameplayEffectContextHandle,
	const FGameplayTag& DamageType)
{
	FGameplayEffectContext* EffectContext=GameplayEffectContextHandle.Get();
	FMyGameplayEffectContext* MyEffectContext=static_cast<FMyGameplayEffectContext*>(EffectContext);
	if (DamageType.IsValid())
	{
		TSharedPtr<FGameplayTag>DamageTypeTag=MakeShared<FGameplayTag>(DamageType);
		MyEffectContext->SetDamageType(DamageTypeTag);
	}
	
}

void UGAST_AbilitySystemLibrary::SetDeathImpulse(FGameplayEffectContextHandle& GameplayEffectContextHandle,
	const FVector& InDeathImpulse)
{
	FGameplayEffectContext* EffectContext=GameplayEffectContextHandle.Get();
	FMyGameplayEffectContext* MyEffectContext=static_cast<FMyGameplayEffectContext*>(EffectContext);
	if (MyEffectContext)
	{
		MyEffectContext->SetDeathImpulse(InDeathImpulse);
	}
}

void UGAST_AbilitySystemLibrary::SetKnockBackForce(FGameplayEffectContextHandle& GameplayEffectContextHandle,
	const FVector& InKnockBackForce)
{
	FGameplayEffectContext* EffectContext=GameplayEffectContextHandle.Get();
	FMyGameplayEffectContext* MyEffectContext=static_cast<FMyGameplayEffectContext*>(EffectContext);
	if (MyEffectContext)
	{
		MyEffectContext->SetKnockBackForce(InKnockBackForce);
	}
}

FGameplayEffectContextHandle UGAST_AbilitySystemLibrary::ApplyDamageEffectToTarget(const FDamageEffectParams& DamageEffectParams)
{
	const FGameplayTags& GameplayTags=FGameplayTags::Get();
	AActor* SourceActor=DamageEffectParams.SourceASC->GetAvatarActor();
	
	FGameplayEffectContextHandle Context= DamageEffectParams.SourceASC->MakeEffectContext();
	Context.AddSourceObject(SourceActor);
	
	SetDeathImpulse(Context,DamageEffectParams.DeathImpulse);
	SetKnockBackForce(Context,DamageEffectParams.KnockBackForce);
	
	FGameplayEffectSpecHandle SpecHandle= DamageEffectParams.SourceASC->MakeOutgoingSpec(DamageEffectParams.DamageEffectClass,DamageEffectParams.AbilityLevel,Context);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,DamageEffectParams.DamageType,DamageEffectParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,GameplayTags.Debuff_Chance,DamageEffectParams.Debuff_Chance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,GameplayTags.Debuff_Damage,DamageEffectParams.Debuff_Damage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,GameplayTags.Debuff_Duration,DamageEffectParams.Debuff_Duration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,GameplayTags.Debuff_Frequency,DamageEffectParams.Debuff_Frequency);
	
	DamageEffectParams.TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	return Context;
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

void UGAST_AbilitySystemLibrary::AsyncLoadImage(const FString& Path)
{
	AsyncTask(ENamedThreads::Type::AnyBackgroundThreadNormalTask,[Path]()
	{
		TArray<uint8> Data;
		if (FFileHelper::LoadFileToArray(Data, *Path))
		{
			UTexture2D* Tex=FImageUtils::ImportBufferAsTexture2D(Data);

			AsyncTask(ENamedThreads::Type::GameThread,[Tex,Path]()
			{
				
			});
		}
	});
}

