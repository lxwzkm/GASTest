// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemode/GAST_PlayerState.h"

#include "AbilitySystemComponent.h"

AGAST_PlayerState::AGAST_PlayerState()
{
	//实例化ASC组件和属性集
	AbilitySystemComponent=CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet=CreateDefaultSubobject<UAttributeSet>("AttributeSet");
	
	NetUpdateFrequency=100.f;//服务器网络的更新频率设置
}

UAbilitySystemComponent* AGAST_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AGAST_PlayerState::GetAttributeSet() const
{
	return AttributeSet;
}
