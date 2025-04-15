// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayTag/GAST_GameplayTags.h"
#include "GameplayTagsManager.h"

void FGameplayTags::InitializeGameplayTags()
{

	/*------------------- Primary Attributes Tag --------------------*/
	GameplayTags.Attributes_Primary_Strength=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Strength"),FString("力量"));
	GameplayTags.Attributes_Primary_Intelligence=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Intrlligence"),FString("智力"));
	GameplayTags.Attributes_Primary_Resilience=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Resilience"),FString("防御力"));
	GameplayTags.Attributes_Primary_Vigor=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Vigor"),FString("活力"));

	
	/*------------------- Secondary Attributes Tag --------------------*/
	GameplayTags.Attributes_Secondary_Armor=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"),FString("护甲"));
	GameplayTags.Attributes_Secondary_ArmorPenetration=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ArmorPenetration"),FString("护甲穿透"));
	GameplayTags.Attributes_Secondary_BlockChance=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.BlockChance"),FString("格挡几率"));
	GameplayTags.Attributes_Secondary_CriticalHitChance=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitChance"),FString("暴击几率"));
	GameplayTags.Attributes_Secondary_CriticalHitDamage=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitDamage"),FString("暴击伤害"));
	GameplayTags.Attributes_Secondary_CriticalHitResistance=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitResistance"),FString("暴击抗性"));
	GameplayTags.Attributes_Secondary_HealthRegeneration=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.HealthRegeneration"),FString("血量成长"));
	GameplayTags.Attributes_Secondary_ManaRegeneration=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ManaRegeneration"),FString("法力成长"));
	GameplayTags.Attributes_Secondary_MaxHealth=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"),FString("最大生命值"));
	GameplayTags.Attributes_Secondary_MaxMana=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxMana"),FString("最大法力值"));

	/*------------------- Resistance Attributes Tag --------------------*/
	GameplayTags.Attributes_Resistance_Fire=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Fire"),FString("火系抗性"));
	GameplayTags.Attributes_Resistance_Lightning=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Lightning"),FString("雷系抗性"));
	GameplayTags.Attributes_Resistance_Arcane=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Arcane"),FString("奥术抗性"));
	GameplayTags.Attributes_Resistance_Physical=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Physical"),FString("物理抗性"));

	/*------------------- InputAction Tag --------------------*/
	GameplayTags.Input_1=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.1"),FString("按键1"));
	GameplayTags.Input_2=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.2"),FString("按键2"));
	GameplayTags.Input_3=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.3"),FString("按键3"));
	GameplayTags.Input_4=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.4"),FString("按键4"));
	GameplayTags.Input_LMB=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.LMB"),FString("鼠标左键"));
	GameplayTags.Input_RMB=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.RMB"),FString("鼠标右键"));

	/*---------------------------------Damage Type------------------------------*/
	GameplayTags.Damage=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"),FString("伤害"));
	GameplayTags.Damage_Fire=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Fire"),FString("火系伤害"));
	GameplayTags.Damage_Lightning=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Lightning"),FString("雷系伤害"));
	GameplayTags.Damage_Arcane=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Arcane"),FString("奥术伤害"));
	GameplayTags.Damage_Physical=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Physical"),FString("物理伤害"));

	/*----------------------------------DamageTypesToResistance----------------------------*/
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Fire,GameplayTags.Attributes_Resistance_Fire);
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Lightning,GameplayTags.Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Arcane,GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Physical,GameplayTags.Attributes_Resistance_Physical);

	GameplayTags.Effect_HitReact=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effect.HitReact"),FString("受击反应标签"));

	GameplayTags.Ability_Attack=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Attack"),FString("物理攻击标签"));
	/*----------------------------------GetMontageSocketTag----------------------------*/
	GameplayTags.Montage_Attack_Weapon=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.Weapon"),FString("武器攻击蒙太奇动画"));
	GameplayTags.Montage_Attack_LeftHand=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.LeftHand"),FString("左手攻击蒙太奇动画"));
	GameplayTags.Montage_Attack_RightHand=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.RightHand"),FString("右手攻击蒙太奇动画"));

}
