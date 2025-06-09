// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecuCalcu/ExecuCalcu_Damage.h"
#include "AbilitySystem/GAST_AttributeSet.h"

#include "AbilitySystemComponent.h"
#include "GAST_AbilitySystemLibrary.h"
#include "GAST_AbilityType.h"
#include "GameplayTag/GAST_GameplayTags.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"

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

void UExecuCalcu_Damage::DetemineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec OwningSpec, FAggregatorEvaluateParameters EvaluateParameters,TMap<FGameplayTag,FGameplayEffectAttributeCaptureDefinition>TagToDefs) const
{
	FGameplayTags GameplayTags=FGameplayTags::Get();
	for (auto Pair:GameplayTags.DamageTypesToDebuff)
	{
		FGameplayTag DamageType=Pair.Key;
		FGameplayTag Debuff=Pair.Value;
		float DamageTypeValue=OwningSpec.GetSetByCallerMagnitude(Pair.Key,false,-1);
		if (DamageTypeValue>-1)
		{
			float DebuffChance=OwningSpec.GetSetByCallerMagnitude(GameplayTags.Debuff_Chance,false,-1);
			float DamgeTypeResistance=0;
			FGameplayTag ResistanceTag=GameplayTags.DamageTypesToResistance[DamageType];
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(TagToDefs[ResistanceTag],EvaluateParameters,DamgeTypeResistance);
			DamgeTypeResistance=FMath::Max(0,DamgeTypeResistance);
			const float DebuffChanceReal=DebuffChance*(100-DamgeTypeResistance)/100;
			const bool bDebuff=FMath::RandRange(0,100)<DebuffChanceReal;
			if (bDebuff)
			{
				FGameplayEffectContextHandle ContextHandle=OwningSpec.GetContext();
				UGAST_AbilitySystemLibrary::SetIsSuccessfulDebuff(ContextHandle,bDebuff);

				float DebuffDamage=OwningSpec.GetSetByCallerMagnitude(GameplayTags.Debuff_Damage,false,-1);
				float DebuffDuration=OwningSpec.GetSetByCallerMagnitude(GameplayTags.Debuff_Duration,false,-1);
				float DebuffFrequency=OwningSpec.GetSetByCallerMagnitude(GameplayTags.Debuff_Frequency,false,-1);
				UGAST_AbilitySystemLibrary::SetDebuffDamage(ContextHandle,DebuffDamage);
				UGAST_AbilitySystemLibrary::SetDebuffDuration(ContextHandle,DebuffDuration);
				UGAST_AbilitySystemLibrary::SetDebuffFrequency(ContextHandle,DebuffFrequency);
				UGAST_AbilitySystemLibrary::SetDamageType(ContextHandle,DamageType);
			}
		}
	}
}

void UExecuCalcu_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{

	TMap<FGameplayTag,FGameplayEffectAttributeCaptureDefinition>TagsToCaptureDefin;
	TagsToCaptureDefin.Add(FGameplayTags::Get().Attributes_Secondary_Armor,MyDamageStatics().ArmorDef);
	TagsToCaptureDefin.Add(FGameplayTags::Get().Attributes_Secondary_ArmorPenetration,MyDamageStatics().ArmorPenetrationDef);
	TagsToCaptureDefin.Add(FGameplayTags::Get().Attributes_Secondary_BlockChance,MyDamageStatics().BlockChanceDef);
	TagsToCaptureDefin.Add(FGameplayTags::Get().Attributes_Secondary_CriticalHitChance,MyDamageStatics().CriticalHitChanceDef);
	TagsToCaptureDefin.Add(FGameplayTags::Get().Attributes_Secondary_CriticalHitDamage,MyDamageStatics().CriticalHitDamageDef);
	TagsToCaptureDefin.Add(FGameplayTags::Get().Attributes_Secondary_CriticalHitResistance,MyDamageStatics().CriticalHitResistanceDef);
		
	TagsToCaptureDefin.Add(FGameplayTags::Get().Attributes_Resistance_Fire,MyDamageStatics().FireResistanceDef);
	TagsToCaptureDefin.Add(FGameplayTags::Get().Attributes_Resistance_Lightning,MyDamageStatics().LightningResistanceDef);
	TagsToCaptureDefin.Add(FGameplayTags::Get().Attributes_Resistance_Arcane,MyDamageStatics().ArcaneResistanceDef);
	TagsToCaptureDefin.Add(FGameplayTags::Get().Attributes_Resistance_Physical,MyDamageStatics().PhysicalResistanceDef);
	
	
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
	FGameplayEffectContextHandle GameplayEffectContextHandle= OwningSpec.GetContext();
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags=OwningSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags=OwningSpec.CapturedTargetTags.GetAggregatedTags();

	//获取SetByCall的伤害
	DetemineDebuff(ExecutionParams, OwningSpec, EvaluateParameters,TagsToCaptureDefin);

	
	float Damage=0.f;
	for (const auto pair:FGameplayTags::Get().DamageTypesToResistance)
	{
		FGameplayTag DamageTypeTag=pair.Key;
		FGameplayTag ResistanceTag=pair.Value;

		float TempDamage= OwningSpec.GetSetByCallerMagnitude(DamageTypeTag,false);//暂时缺少其他类型的伤害
		
		FGameplayEffectAttributeCaptureDefinition CaptureDefinition=TagsToCaptureDefin[ResistanceTag];
		float Resistance=0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDefinition,EvaluateParameters,Resistance);
		Resistance=FMath::Clamp(Resistance,0.f,100.f);

		TempDamage*=(100.f-Resistance)/100.f;
		if (TempDamage<=0.f)continue;
		//进行径向伤害的判断和计算
		//1.重写TakeDamage函数在CharacterBase上
		//2.在接口中创建一个委托，并增加一个获取的纯虚函数
		//3.重写获取函数，并在TakeDamage中广播伤害
		//4.绑定Lambda函数接收结果
		//5.ApplyRadialDamage
		if (UGAST_AbilitySystemLibrary::GetbIsRadialDamage(GameplayEffectContextHandle))
		{
			if (ICombatInterface* CombatInterface=Cast<ICombatInterface>(TargetAvatar))
			{
				CombatInterface->GetRaidalDamageDelegate().AddLambda([&](float DamageAmount)
				{
					TempDamage=DamageAmount;
				});
			}
			
			UGameplayStatics::ApplyRadialDamageWithFalloff(TargetAvatar,TempDamage,0.f,
				UGAST_AbilitySystemLibrary::GetRadialDamageOrigin(GameplayEffectContextHandle),
				UGAST_AbilitySystemLibrary::GetRadialDamageInnerRadius(GameplayEffectContextHandle),
				UGAST_AbilitySystemLibrary::GetRadialDamageOuterRadius(GameplayEffectContextHandle),
				1.f,UDamageType::StaticClass(),TArray<AActor*>(),SourAvatar,nullptr);
		}
		
		Damage+=TempDamage;
	}
	//是否被格挡
	float BlockChance=0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetMyDamageStatics().BlockChanceDef,EvaluateParameters,BlockChance);
	BlockChance=FMath::Max<float>(0.f,BlockChance);
	const bool bBlocked=FMath::RandRange(1.f,100.f)<=BlockChance;
	Damage=bBlocked?Damage*=0.5f:Damage;
	
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
