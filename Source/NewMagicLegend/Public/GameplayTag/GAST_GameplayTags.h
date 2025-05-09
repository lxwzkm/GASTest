// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * 单例模式的GameplayTag
 */

struct FGameplayTags
{
public:
	static const FGameplayTags& Get(){return GameplayTags;}
	static void InitializeGameplayTags();

	/*---------Primary Attributes----------*/
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;
	
	/*---------Secondary Attributes----------*/
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;

	/*---------Resistance Attributes----------*/
	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Physical;
	
	/*---------Meta Attributes----------*/
	FGameplayTag Attributes_Meta_IncomingXP;

	/*---------Input Action----------*/
	FGameplayTag Input_LMB;
	FGameplayTag Input_RMB;
	FGameplayTag Input_1;
	FGameplayTag Input_2;
	FGameplayTag Input_3;
	FGameplayTag Input_4;
	FGameplayTag Input_Passive_1;
	FGameplayTag Input_Passive_2;

	/*---------Damage Type----------*/
	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;

	/*---------CombatSocket Tag------------*/
	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_Tail;

	/*---------MontageAttack Tag------------*/
	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;



	TMap<FGameplayTag,FGameplayTag> DamageTypesToResistance;
	
	FGameplayTag Effect_HitReact;

	//AI来激活攻击任务的标签 区分不同技能类型
	FGameplayTag Ability_Attack;
	FGameplayTag Ability_Summom;

	/* ------------ AbilityTag --------------*/
	FGameplayTag Ability_HitReact;

	FGameplayTag Ability_Status_Locked;
	FGameplayTag Ability_Status_Eligible;
	FGameplayTag Ability_Status_Unlocked;
	FGameplayTag Ability_Status_Equipped;

	FGameplayTag Ability_Type_Offsive;
	FGameplayTag Ability_Type_Passive;

	/*---------Cooldown Tag------------*/
	FGameplayTag Cooldown_Fire_FireBolt;
	FGameplayTag Cooldown_Lightning_Electrocute;

	//人物用来确定UI图片的标签
	FGameplayTag Ability_Fire_FireBolt;
	FGameplayTag Ability_Lightning_Electrocute;
	
	
private:
	static FGameplayTags GameplayTags;
};

