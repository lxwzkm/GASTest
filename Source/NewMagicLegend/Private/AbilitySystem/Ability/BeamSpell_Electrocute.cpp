// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/BeamSpell_Electrocute.h"

FString UBeamSpell_Electrocute::GetCurrentLevelDescription(int32 Level)
{
	const float Damage = DamageValue.GetValueAtLevel(Level);
	const float Cost=GetManaCost(Level);
	const float CD=GetCooldown(Level);
	FString Description=FString::Printf(TEXT(
	"<Title>闪电术</>\n\n"
	"<Default>当前等级： </><Level>%d</>\n"
	"<Default>当前法力消耗:</><ManaCost> %.1f </>\n"
	"<Default>当前CD:</><Cooldown> %.1f </>\n\n"
	"<Default>发射 %d 道闪电，"
	"造成雷系伤害</><Damage> %.1f </>\n"
	"<Default>雷系伤害有一定几率造成眩晕效果</>"
	),Level,FMath::Abs(Cost),CD,FMath::Min(Level,MaxNumAdditionalTargets),Damage);
	return Description;
}

FString UBeamSpell_Electrocute::GetNextLevelDescription(int32 Level)
{
	const float Damage = DamageValue.GetValueAtLevel(Level);
	const float Cost=GetManaCost(Level);
	const float CD=GetCooldown(Level);
	FString Description=FString::Printf(TEXT(
		"<Title>闪电术</>\n\n"
		"<Default>下一等级： </><Level>%d</>\n"
		"<Default>法力消耗:</><ManaCost> %.1f </>\n"
		"<Default>CD:</><Cooldown> %.1f </>\n\n"
		"<Default>发射 %d 道闪电，"
		"造成雷系伤害</><Damage> %.1f </>\n"
		"<Default>雷系伤害有一定几率造成眩晕效果</>"
		),Level,FMath::Abs(Cost),CD,FMath::Min(Level,MaxNumAdditionalTargets),Damage);
	return Description;
}
