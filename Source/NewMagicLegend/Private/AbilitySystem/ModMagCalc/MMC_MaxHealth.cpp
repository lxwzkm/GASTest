// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/GAST_AttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorDef.AttributeToCapture=UGAST_AttributeSet::GetVigorAttribute();
	VigorDef.bSnapshot=false;
	VigorDef.AttributeSource=EGameplayEffectAttributeCaptureSource::Target;

	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags=Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags=Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags=SourceTags;
	EvaluateParameters.TargetTags=TargetTags;

	float Vigor=0.f;
	GetCapturedAttributeMagnitude(VigorDef,Spec,EvaluateParameters,Vigor);
	Vigor=FMath::Max(Vigor,0.f);

	ICombatInterface* CombatInterface=Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 Level=CombatInterface->GetPlayerLevel();

	return 80.f+2.5f*Vigor+10.f*Level;
}
