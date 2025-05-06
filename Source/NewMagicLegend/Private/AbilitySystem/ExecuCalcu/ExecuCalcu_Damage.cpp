// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecuCalcu/ExecuCalcu_Damage.h"
#include "AbilitySystem/GAST_AttributeSet.h"

#include "AbilitySystemComponent.h"
#include "GAST_AbilitySystemLibrary.h"
#include "GAST_AbilityType.h"
#include "GameplayTag/GAST_GameplayTags.h"
#include "Interaction/CombatInterface.h"

struct MyDamageStatics
{
	MyDamageStatics()
	{
		/**DECLARE声明变量，DEFINE初始化变量 P属性必须保持一致
		 * S：属性所在的类（如UGAST_AttributeSet）。
		 * P：属性名（如Armor）。
		 * T：捕获来源（Source或Target）。
		 * B：是否在效果应用时立即捕获（bSnapShot）。 
		 */
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGAST_AttributeSet,Armor,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGAST_AttributeSet,ArmorPenetration,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGAST_AttributeSet,BlockChance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGAST_AttributeSet,CriticalHitChance,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGAST_AttributeSet,CriticalHitDamage,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGAST_AttributeSet,CriticalHitResistance,Target,false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UGAST_AttributeSet,FireResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGAST_AttributeSet,LightningResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGAST_AttributeSet,ArcaneResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGAST_AttributeSet,PhysicalResistance,Target,false);

		TagsToCaptureDefin.Add(FGameplayTags::Get().Attributes_Secondary_Armor,ArmorDef);
		TagsToCaptureDefin.Add(FGameplayTags::Get().Attributes_Secondary_ArmorPenetration,ArmorPenetrationDef);
		TagsToCaptureDefin.Add(FGameplayTags::Get().Attributes_Secondary_BlockChance,BlockChanceDef);
		TagsToCaptureDefin.Add(FGameplayTags::Get().Attributes_Secondary_CriticalHitChance,CriticalHitChanceDef);
		TagsToCaptureDefin.Add(FGameplayTags::Get().Attributes_Secondary_CriticalHitDamage,CriticalHitDamageDef);
		TagsToCaptureDefin.Add(FGameplayTags::Get().Attributes_Secondary_CriticalHitResistance,CriticalHitResistanceDef);
		
		TagsToCaptureDefin.Add(FGameplayTags::Get().Attributes_Resistance_Fire,FireResistanceDef);
		TagsToCaptureDefin.Add(FGameplayTags::Get().Attributes_Resistance_Lightning,LightningResistanceDef);
		TagsToCaptureDefin.Add(FGameplayTags::Get().Attributes_Resistance_Arcane,ArcaneResistanceDef);
		TagsToCaptureDefin.Add(FGameplayTags::Get().Attributes_Resistance_Physical,PhysicalResistanceDef);
	}
	/** 为属性P生成两个成员变量：
 * P##Property：指向属性（FProperty）的指针。
 * P##Def：属性捕获定义（FGameplayEffectAttributeCaptureDefinition）。
 */
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration)
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);

	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);

	TMap<FGameplayTag,FGameplayEffectAttributeCaptureDefinition>TagsToCaptureDefin;
	
};

static const MyDamageStatics& GetMyDamageStatics()
{
	static MyDamageStatics Ds;
	return Ds;
}

UExecuCalcu_Damage::UExecuCalcu_Damage()
{
	RelevantAttributesToCapture.Add(GetMyDamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(GetMyDamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(GetMyDamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(GetMyDamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(GetMyDamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(GetMyDamageStatics().CriticalHitResistanceDef);

	RelevantAttributesToCapture.Add(GetMyDamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(GetMyDamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(GetMyDamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(GetMyDamageStatics().PhysicalResistanceDef);

}

void UExecuCalcu_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent*SourceASC=ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent*TargetASC=ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourAvatar=SourceASC?SourceASC->GetAvatarActor():nullptr;
	AActor*TargetAvatar=TargetASC?TargetASC->GetAvatarActor():nullptr;

	int32 SourceLevel=1;
	if (SourAvatar->Implements<UCombatInterface>())
	{
		SourceLevel=ICombatInterface::Execute_GetPlayerLevel(SourAvatar);
	}
	int32 TargetLevel=1;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetLevel=ICombatInterface::Execute_GetPlayerLevel(TargetAvatar);
	}

	const FGameplayEffectSpec OwningSpec= ExecutionParams.GetOwningSpec();
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags=OwningSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags=OwningSpec.CapturedTargetTags.GetAggregatedTags();

	//获取SetByCall的伤害
	float Damage=0.f;
	for (const auto pair:FGameplayTags::Get().DamageTypesToResistance)
	{
		FGameplayTag DamageTypeTag=pair.Key;
		FGameplayTag ResistanceTag=pair.Value;

		float TempDamage= OwningSpec.GetSetByCallerMagnitude(DamageTypeTag,false);//暂时缺少其他类型的伤害
		
		FGameplayEffectAttributeCaptureDefinition CaptureDefinition=MyDamageStatics().TagsToCaptureDefin[ResistanceTag];
		float Resistance=0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDefinition,EvaluateParameters,Resistance);
		Resistance=FMath::Clamp(Resistance,0.f,100.f);

		TempDamage*=(100.f-Resistance)/100.f;
		Damage+=TempDamage;
	}
	//是否被格挡
	float BlockChance=0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetMyDamageStatics().BlockChanceDef,EvaluateParameters,BlockChance);
	BlockChance=FMath::Max<float>(0.f,BlockChance);
	const bool bBlocked=FMath::RandRange(1.f,100.f)<=BlockChance;
	Damage=bBlocked?Damage*=0.5f:Damage;

	FGameplayEffectContextHandle GameplayEffectContextHandle= OwningSpec.GetContext();
	UGAST_AbilitySystemLibrary::SetIsBlockHit(GameplayEffectContextHandle,bBlocked);

	//护甲与护甲穿透的效果应用
	float TargetArmor=0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetMyDamageStatics().ArmorDef,EvaluateParameters,TargetArmor);
	TargetArmor=FMath::Max<float>(0.f,TargetArmor);
	
	float SourceArmorPenetration=0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetMyDamageStatics().ArmorPenetrationDef,EvaluateParameters,SourceArmorPenetration);
	SourceArmorPenetration=FMath::Max<float>(0.f,SourceArmorPenetration);
	//计算护甲与护甲穿透的应用系数
	UCharacterClassInfo* SourceClassInfo= UGAST_AbilitySystemLibrary::GetCharacterClassInfo(SourAvatar);
	FRealCurve*ArmorPenetrationCurve= SourceClassInfo->DamageCoefficient->FindCurve(FName("ArmorPenetration"),FString());
	const float ArmorPenetrationCoefficient= ArmorPenetrationCurve->Eval(SourceLevel);

	UCharacterClassInfo*TargetClassInfo=UGAST_AbilitySystemLibrary::GetCharacterClassInfo(TargetAvatar);
	FRealCurve*EffectArmorCurve=TargetClassInfo->DamageCoefficient->FindCurve(FName("EffectArmor"),FString());
	const float EffectArmorCoeffiencient=EffectArmorCurve->Eval(TargetLevel);
	
	const float EffectArmor=TargetArmor*(100.f-SourceArmorPenetration*ArmorPenetrationCoefficient)/100.f;
	Damage*=(100-EffectArmor*EffectArmorCoeffiencient)/100;

	//是否可以暴击
	float SourceCriticalHitChance=0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetMyDamageStatics().CriticalHitChanceDef,EvaluateParameters,SourceCriticalHitChance);
	SourceCriticalHitChance=FMath::Max<float>(0.f,SourceCriticalHitChance);

	float TargetCriticalHitResistance=0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetMyDamageStatics().CriticalHitResistanceDef,EvaluateParameters,TargetCriticalHitResistance);
	TargetCriticalHitResistance=FMath::Max<float>(0.f,TargetCriticalHitResistance);

	float SourceCriticalHitDamage=0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetMyDamageStatics().CriticalHitDamageDef,EvaluateParameters,SourceCriticalHitDamage);
	SourceCriticalHitDamage=FMath::Max<float>(0.f,SourceCriticalHitDamage);

	//获取暴击几率的系数
	FRealCurve* SourceCriticalHitChanceCoefficient=SourceClassInfo->DamageCoefficient->FindCurve(FName("CriticalHitResistance"),FString());
	const float SourceCriticalCoefficient= SourceCriticalHitChanceCoefficient->Eval(TargetLevel);

	//暴击伤害计算
	const float EffectCriticalChance=SourceCriticalHitChance-TargetCriticalHitResistance*SourceCriticalCoefficient;
	const bool bCritical=FMath::RandRange(1,100)<=EffectCriticalChance;
	if (bCritical)
	{
		Damage=Damage*2+SourceCriticalHitDamage;
	}

	UGAST_AbilitySystemLibrary::SetIsCriticalHit(GameplayEffectContextHandle,bCritical);

	FGameplayModifierEvaluatedData EvaluatedData(UGAST_AttributeSet::GetIncomingDamageAttribute(),EGameplayModOp::Additive,Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
