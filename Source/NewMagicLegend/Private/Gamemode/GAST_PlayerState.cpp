// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemode/GAST_PlayerState.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/GAST_AttributeSet.h"
#include "Net/UnrealNetwork.h"

AGAST_PlayerState::AGAST_PlayerState()
{
	//实例化ASC组件和属性集
	AbilitySystemComponent=CreateDefaultSubobject<UGAST_AbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet=CreateDefaultSubobject<UGAST_AttributeSet>("AttributeSet");
	
	NetUpdateFrequency=100.f;//服务器网络的更新频率设置
}

void AGAST_PlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGAST_PlayerState,Level);//将Level标记为可以被复制的
	DOREPLIFETIME(AGAST_PlayerState,XP);
	DOREPLIFETIME(AGAST_PlayerState,SpellPoints);
	DOREPLIFETIME(AGAST_PlayerState,AttributePoints);
}

UAbilitySystemComponent* AGAST_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AGAST_PlayerState::GetAttributeSet() const
{
	return AttributeSet;
}
void AGAST_PlayerState::SetLevel(int32 InLevel)
{
	Level=InLevel;
	OnLevelChangeDelegate.Broadcast(Level);
}

void AGAST_PlayerState::AddToLevel(int32 InLevel)
{
	Level+=InLevel;
	OnLevelChangeDelegate.Broadcast(Level);
}

void AGAST_PlayerState::AddToSpelPoints(int32 InSpellPoints)
{
	SpellPoints+=InSpellPoints;
	OnSpellPointsChangeDelegate.Broadcast(SpellPoints);
}

void AGAST_PlayerState::AddToAttributePoints(int32 InAttributePoints)
{
	AttributePoints+=InAttributePoints;
	OnAttributePointsChangeDelegate.Broadcast(AttributePoints);
}

void AGAST_PlayerState::SetXP(int32 InXP)
{
	XP=InXP;
	OnXPChangeDelegate.Broadcast(XP);
}

void AGAST_PlayerState::AddToXP(int32 InXP)
{
	XP+=InXP;
	OnXPChangeDelegate.Broadcast(XP);
}

void AGAST_PlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangeDelegate.Broadcast(Level);
}

void AGAST_PlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangeDelegate.Broadcast(XP);
}

void AGAST_PlayerState::OnRep_SpellPoints(int32 OldXP)
{
	OnSpellPointsChangeDelegate.Broadcast(SpellPoints);
}

void AGAST_PlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
	OnAttributePointsChangeDelegate.Broadcast(AttributePoints);
}