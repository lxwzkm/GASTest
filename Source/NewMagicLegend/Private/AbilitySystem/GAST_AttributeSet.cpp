// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GAST_AttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"//FGameplayEffectModCallbackData类型必须包含该头文件
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
UGAST_AttributeSet::UGAST_AttributeSet()
{
	InitHealth(50.f);
	InitMaxHealth(100.f);
	InitMana(10.f);
	InitMaxMana(50.f);
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

void UGAST_AttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//控制传递的参数，将新值从服务器传递给客户端
	DOREPLIFETIME_CONDITION_NOTIFY(UGAST_AttributeSet,Health,COND_None,REPNOTIFY_Always);//参数1 类名  参数2 传递的属性名  参数3 复制不需要任何条件  参数4 每次属性变化时都通知客户端
	DOREPLIFETIME_CONDITION_NOTIFY(UGAST_AttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAST_AttributeSet,Mana,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAST_AttributeSet,MaxMana,COND_None,REPNOTIFY_Always);
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
