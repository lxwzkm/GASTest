// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Damage_ArcaneShards.h"

FString UDamage_ArcaneShards::GetCurrentLevelDescription(int32 Level)
{
	const float Damage = DamageValue.GetValueAtLevel(Level);
	const float Cost=GetManaCost(Level);
	const float CD=GetCooldown(Level);
	FString Description=FString::Printf(TEXT(
	"<Title>奥术碎片</>\n\n"
	"<Default>当前等级： </><Level>%d</>\n"
	"<Default>当前法力消耗:</><ManaCost> %.1f </>\n"
	"<Default>当前CD:</><Cooldown> %.1f </>\n\n"
	"<Default>产生 %d 个奥术碎片，"
	"造成奥术伤害</><Damage> %.1f </>\n"
	"<Default>奥术伤害等级上限更高</>"
	),Level,FMath::Abs(Cost),CD,FMath::Min(Level,MaxSpawnNum),Damage);
	return Description;
}

FString UDamage_ArcaneShards::GetNextLevelDescription(int32 Level)
{
	const float Damage = DamageValue.GetValueAtLevel(Level);
	const float Cost=GetManaCost(Level);
	const float CD=GetCooldown(Level);
	FString Description=FString::Printf(TEXT(
	"<Title>奥术碎片</>\n\n"
	"<Default>当前等级： </><Level>%d</>\n"
	"<Default>当前法力消耗:</><ManaCost> %.1f </>\n"
	"<Default>当前CD:</><Cooldown> %.1f </>\n\n"
	"<Default>产生 %d 个奥术碎片，"
	"造成奥术伤害</><Damage> %.1f </>\n"
	"<Default>奥术伤害等级上限更高</>"
	),Level,FMath::Abs(Cost),CD,FMath::Min(Level,MaxSpawnNum),Damage);
	return Description;
}
