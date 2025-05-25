// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/DamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GAST_AbilitySystemLibrary.h"

void UDamageGameplayAbility::CauseDamageToTarget(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle= MakeOutgoingGameplayEffectSpec(DamageEffectClass,1.f);

	float DamageMagnitude=DamageValue.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle,DamageType,DamageMagnitude);
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(),UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

FDamageEffectParams UDamageGameplayAbility::MakeDamageParams(AActor* TargetActor)
{
	FDamageEffectParams DamageParams;
	DamageParams.WorldContext=GetAvatarActorFromActorInfo();
	DamageParams.SourceASC=GetAbilitySystemComponentFromActorInfo();
	DamageParams.AbilityLevel=GetAbilityLevel();
	DamageParams.BaseDamage=DamageValue.GetValueAtLevel(20);//GetAbilityLevel()
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
		ToTarget.Pitch=45.f;
		FVector KnockBackForce=ToTarget.Vector();
		DamageParams.KnockBackForce=KnockBackForce * DamageParams.KnockBackMagnitude;
		DamageParams.DeathImpulse=KnockBackForce * DamageParams.KnockBackMagnitude;
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
