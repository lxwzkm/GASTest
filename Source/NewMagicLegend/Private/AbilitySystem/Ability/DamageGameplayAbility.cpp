// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/DamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GAST_AbilitySystemLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void UDamageGameplayAbility::CauseDamageToTarget(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle= MakeOutgoingGameplayEffectSpec(DamageEffectClass,1.f);

	float DamageMagnitude=DamageValue.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle,DamageType,DamageMagnitude);
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(),UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

FDamageEffectParams UDamageGameplayAbility::MakeDamageParams(AActor* TargetActor, FVector RadialDamageOrigin,
	bool bKnockBackOverride, FVector KnockBackDirectionOverride, bool bDeathImpluseOverride,
	FVector DeathDirectionOverride, bool bPitchOverride, float PitchOverride)
{
	FDamageEffectParams DamageParams;
	DamageParams.WorldContext=GetAvatarActorFromActorInfo();
	DamageParams.SourceASC=GetAbilitySystemComponentFromActorInfo();
	DamageParams.AbilityLevel=GetAbilityLevel();
	DamageParams.BaseDamage=DamageValue.GetValueAtLevel(GetAbilityLevel());//GetAbilityLevel()
	DamageParams.DamageType=DamageType;
	DamageParams.TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	DamageParams.Debuff_Chance=Debuff_Chance;
	DamageParams.Debuff_Damage=Debuff_Damage;
	DamageParams.Debuff_Duration=Debuff_Duration;
	DamageParams.Debuff_Frequency=Debuff_Frequency;
	DamageParams.DamageEffectClass=DamageEffectClass;
	DamageParams.DeathImpulseMagnitude=DeathImpulseMagnitude;
	DamageParams.KnockBackMagnitude=KnockBackMagnitude;
	DamageParams.KnockBackChance=KnockBackChance;
	
	if (IsValid(TargetActor))
	{
		FRotator ToTarget=(TargetActor->GetActorLocation()-GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		FVector KnockBackForce=ToTarget.Vector();
		if (bPitchOverride)
		{
			ToTarget.Pitch=PitchOverride;
		}
		if (!bKnockBackOverride)
		{
			DamageParams.KnockBackForce=KnockBackForce * KnockBackMagnitude;
		}
		if (!bDeathImpluseOverride)
		{
			DamageParams.DeathImpulse=KnockBackForce * DeathImpulseMagnitude;
		}
	}

	if (bKnockBackOverride)
	{
		KnockBackDirectionOverride.Normalize();
		DamageParams.KnockBackForce=KnockBackDirectionOverride * KnockBackMagnitude;
		if (bPitchOverride)
		{
			FRotator Rotation= KnockBackDirectionOverride.Rotation();
			Rotation.Pitch=PitchOverride;
			DamageParams.KnockBackForce=Rotation.Vector()*KnockBackMagnitude;
		}
	}
	if (bDeathImpluseOverride)
	{
		DeathDirectionOverride.Normalize();
		DamageParams.DeathImpulse=DeathDirectionOverride*DeathImpulseMagnitude;
		if (bPitchOverride)
		{
			FRotator Rotation= DeathDirectionOverride.Rotation();
			Rotation.Pitch=PitchOverride;
			DamageParams.DeathImpulse=Rotation.Vector()*DeathImpulseMagnitude;
		}
	}
	
	if (bIsRadialDamge)
	{
		DamageParams.bIsRadialDamge=bIsRadialDamge;
		DamageParams.RadialDamageInnerRadius=RadialDamageInnerRadius;
		DamageParams.RadialDamageOuterRadius=RadialDamageOuterRadius;
		DamageParams.RadialDamageOrigin=RadialDamageOrigin;
	}
	return DamageParams;
}

float UDamageGameplayAbility::GetDamageAtLevel()
{
	return DamageValue.GetValueAtLevel(GetAbilityLevel());
}

FTagMontage UDamageGameplayAbility::GetRandomTagMontage(const TArray<FTagMontage>& TagMontages)
{
	if (TagMontages.Num()>0)
	{
		const int32 Selection=FMath::RandRange(0,TagMontages.Num()-1);
		return TagMontages[Selection];
	}
	return FTagMontage();
}
