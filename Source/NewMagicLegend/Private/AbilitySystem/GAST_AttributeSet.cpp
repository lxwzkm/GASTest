// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GAST_AttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"//FGameplayEffectModCallbackData类型必须包含该头文件
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
UGAST_AttributeSet::UGAST_AttributeSet()
{
	InitHealth(50.f);
	InitMana(10.f);
}

void UGAST_AttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAST_AttributeSet,Health,OldHealth);
}

void UGAST_AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAST_AttributeSet,MaxHealth,OldMaxHealth);
}

void UGAST_AttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAST_AttributeSet,Mana,OldMana);
}

void UGAST_AttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAST_AttributeSet,MaxMana,OldMaxMana);
}
/*------------------------------------------------------------------------------------o*/
void UGAST_AttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAST_AttributeSet,Strength,OldStrength);
}

void UGAST_AttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAST_AttributeSet,Intelligence,OldIntelligence);
}

void UGAST_AttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAST_AttributeSet,Resilience,OldResilience);
}

void UGAST_AttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAST_AttributeSet,Vigor,OldVigor);
}

void UGAST_AttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAST_AttributeSet,Armor,OldArmor);
}

void UGAST_AttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAST_AttributeSet,ArmorPenetration,OldArmorPenetration);
}

void UGAST_AttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAST_AttributeSet,BlockChance,OldBlockChance);
}

void UGAST_AttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAST_AttributeSet,BlockChance,OldCriticalHitChance);
}

void UGAST_AttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAST_AttributeSet,CriticalHitDamage,OldCriticalHitDamage);
}

void UGAST_AttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAST_AttributeSet,CriticalHitResistance,OldCriticalHitResistance);
}

void UGAST_AttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAST_AttributeSet,HealthRegeneration,OldHealthRegeneration);
}

void UGAST_AttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAST_AttributeSet,ManaRegeneration,OldManaRegeneration);
}

void UGAST_AttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//控制传递的参数，将新值从服务器传递给客户端
	DOREPLIFETIME_CONDITION_NOTIFY(UGAST_AttributeSet,Health,COND_None,REPNOTIFY_Always);//参数1 类名  参数2 传递的属性名  参数3 复制不需要任何条件  参数4 每次属性变化时都通知客户端
	DOREPLIFETIME_CONDITION_NOTIFY(UGAST_AttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAST_AttributeSet,Mana,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAST_AttributeSet,MaxMana,COND_None,REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UGAST_AttributeSet,Strength,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAST_AttributeSet,Intelligence,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAST_AttributeSet,Resilience,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAST_AttributeSet,Vigor,COND_None,REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UGAST_AttributeSet,Armor,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAST_AttributeSet,ArmorPenetration,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAST_AttributeSet,BlockChance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAST_AttributeSet,CriticalHitChance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAST_AttributeSet,CriticalHitDamage,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAST_AttributeSet,CriticalHitResistance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAST_AttributeSet,HealthRegeneration,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAST_AttributeSet,ManaRegeneration,COND_None,REPNOTIFY_Always);
}

void UGAST_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	if (Attribute==GetHealthAttribute())
	{
		NewValue=FMath::Clamp(NewValue,0.f,GetMaxHealth());
	}
	if (Attribute==GetManaAttribute())
	{
		NewValue=FMath::Clamp(NewValue,0.f,GetMaxMana());
	}
	
}

void UGAST_AttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties EffectProperties;
	SetEffectPropertiesByData(Data,EffectProperties);//将Source和Target数据封装成一个结构体，并依据Data内的数据设置好它

	if (Data.EvaluatedData.Attribute==GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(),0.f,GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute==GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(),0.f,GetMaxMana()));
	}
}

void UGAST_AttributeSet::SetEffectPropertiesByData(const FGameplayEffectModCallbackData& Data,
                                                   FEffectProperties& Props)
{
	
	
	Props.GameplayEffectContextHandle=Data.EffectSpec.GetContext();
	Props.SourceASC=Data.EffectSpec.GetContext().GetOriginalInstigatorAbilitySystemComponent();
	if (IsValid(Props.SourceASC)&&Props.SourceASC->AbilityActorInfo.IsValid()&&IsValid(Props.SourceASC->AbilityActorInfo->AvatarActor.Get()))
	{
		Props.SourceAvatarActor=Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController=Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController==nullptr&&Props.SourceAvatarActor!=nullptr)
		{
			if (APawn* SourceActor=Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController=SourceActor->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter=Props.SourceController->GetCharacter();//Props.SourceCharacter=Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	if (IsValid(&Data.Target)&&Data.Target.AbilityActorInfo.IsValid()&&Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor=Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController=Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
		Props.TargetCharacter=Cast<ACharacter>(Props.TargetAvatarActor);
	}

	
}
