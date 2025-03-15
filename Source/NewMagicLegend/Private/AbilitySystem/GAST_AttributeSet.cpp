// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GAST_AttributeSet.h"
#include "Net/UnrealNetwork.h"

UGAST_AttributeSet::UGAST_AttributeSet()
{
	InitHealth(100.f);
	InitMaxHealth(100.f);
	InitMana(50.f);
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
