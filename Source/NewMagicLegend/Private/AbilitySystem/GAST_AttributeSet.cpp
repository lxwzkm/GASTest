// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GAST_AttributeSet.h"
#include "GameplayTag/GAST_GameplayTags.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"//FGameplayEffectModCallbackData类型必须包含该头文件
#include "GAST_AbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Gamemode/GAST_PlayerCOntroller.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

UGAST_AttributeSet::UGAST_AttributeSet()
{
	TagsToAttribute.Add( FGameplayTags::Get().Attributes_Primary_Strength,GetStrengthAttribute);
	TagsToAttribute.Add( FGameplayTags::Get().Attributes_Primary_Intelligence,GetIntelligenceAttribute);
	TagsToAttribute.Add( FGameplayTags::Get().Attributes_Primary_Resilience,GetResilienceAttribute);
	TagsToAttribute.Add( FGameplayTags::Get().Attributes_Primary_Vigor,GetVigorAttribute);

	TagsToAttribute.Add( FGameplayTags::Get().Attributes_Secondary_Armor,GetArmorAttribute);
	TagsToAttribute.Add( FGameplayTags::Get().Attributes_Secondary_ArmorPenetration,GetArmorPenetrationAttribute);
	TagsToAttribute.Add( FGameplayTags::Get().Attributes_Secondary_BlockChance,GetBlockChanceAttribute);
	TagsToAttribute.Add( FGameplayTags::Get().Attributes_Secondary_CriticalHitChance,GetCriticalHitChanceAttribute);
	TagsToAttribute.Add( FGameplayTags::Get().Attributes_Secondary_CriticalHitDamage,GetCriticalHitDamageAttribute);
	TagsToAttribute.Add( FGameplayTags::Get().Attributes_Secondary_CriticalHitResistance,GetCriticalHitResistanceAttribute);
	TagsToAttribute.Add( FGameplayTags::Get().Attributes_Secondary_HealthRegeneration,GetHealthRegenerationAttribute);
	TagsToAttribute.Add( FGameplayTags::Get().Attributes_Secondary_ManaRegeneration,GetManaRegenerationAttribute);
	TagsToAttribute.Add( FGameplayTags::Get().Attributes_Secondary_MaxHealth,GetMaxHealthAttribute);
	TagsToAttribute.Add( FGameplayTags::Get().Attributes_Secondary_MaxMana,GetMaxManaAttribute);

	TagsToAttribute.Add(FGameplayTags::Get().Attributes_Resistance_Fire,GetFireResistanceAttribute);
	TagsToAttribute.Add(FGameplayTags::Get().Attributes_Resistance_Lightning,GetLightningResistanceAttribute);
	TagsToAttribute.Add(FGameplayTags::Get().Attributes_Resistance_Arcane,GetArcaneResistanceAttribute);
	TagsToAttribute.Add(FGameplayTags::Get().Attributes_Resistance_Physical,GetPhysicalResistanceAttribute);
}

#pragma region OnRep_函数
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

void UGAST_AttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAST_AttributeSet,FireResistance,OldFireResistance);
}

void UGAST_AttributeSet::OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAST_AttributeSet,LightningResistance,OldLightningResistance);
}


void UGAST_AttributeSet::OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAST_AttributeSet,ArcaneResistance,OldArcaneResistance);
}

void UGAST_AttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAST_AttributeSet,PhysicalResistance,OldPhysicalResistance);
}

/*------------------------------------------------------------------------------------*/
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
#pragma endregion 

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

	DOREPLIFETIME_CONDITION_NOTIFY(UGAST_AttributeSet,FireResistance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAST_AttributeSet,LightningResistance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAST_AttributeSet,ArcaneResistance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAST_AttributeSet,PhysicalResistance,COND_None,REPNOTIFY_Always);
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

void UGAST_AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
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

	if (Data.EvaluatedData.Attribute==GetIncomingDamageAttribute())
	{
		const float LocalDamage=GetIncomingDamage();
		SetIncomingDamage(0.f);
		if (LocalDamage>0.f)
		{
			const float NewHealth=GetHealth()-LocalDamage;
			SetHealth(FMath::Clamp(NewHealth,0.f,GetMaxHealth()));

			const bool bFatal=NewHealth<=0.f;//是否是致命伤害
			if (!bFatal)
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FGameplayTags::Get().Effect_HitReact);
				EffectProperties.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}
			else
			{
				ICombatInterface* CombatInterface= Cast<ICombatInterface>(EffectProperties.TargetAvatarActor);
				if (CombatInterface)
				{
					CombatInterface->Die();
					SendXPReward(EffectProperties);
				}
			}
			
			const bool bIsBlockedHit=UGAST_AbilitySystemLibrary::IsBlockedHit(EffectProperties.GameplayEffectContextHandle);
			const bool bIsCriticalHit=UGAST_AbilitySystemLibrary::IsCriticalHit(EffectProperties.GameplayEffectContextHandle);
			ShowFloatingText(EffectProperties,LocalDamage,bIsBlockedHit,bIsCriticalHit);
		}
	}
	if (Data.EvaluatedData.Attribute==GetInComingXPAttribute())
	{
		const int32 LocalXP=GetInComingXP();
		SetInComingXP(0.f);
		if (LocalXP>0&&EffectProperties.SourceCharacter->Implements<UPlayerInterface>()&&EffectProperties.SourceCharacter->Implements<UPlayerInterface>())
		{
			int32 CurrentLevel=ICombatInterface::Execute_GetPlayerLevel(EffectProperties.SourceCharacter);
			int32 CurrentXP=IPlayerInterface::Execute_GetXP(EffectProperties.SourceCharacter);

			int32 NewLevel=IPlayerInterface::Execute_FindLevelForXP(EffectProperties.SourceCharacter,CurrentXP + LocalXP);
			int32 DeltaLevel=NewLevel - CurrentLevel;
			if (DeltaLevel>0)
			{
				IPlayerInterface::Execute_AddToLevel(EffectProperties.SourceCharacter,DeltaLevel);

				int32 AttributePointsReward=0;
				int32 SpellPointsReward=0;
				for (int32 i=0;i<DeltaLevel;i++)
				{
					AttributePointsReward += IPlayerInterface::Execute_GetAttributePointsReward(EffectProperties.SourceCharacter,CurrentLevel+i);
					SpellPointsReward += IPlayerInterface::Execute_GetSpellPointsReward(EffectProperties.SourceCharacter,CurrentLevel+i);
				}
				
				IPlayerInterface::Execute_AddToAttributePoints(EffectProperties.SourceCharacter,AttributePointsReward);
				IPlayerInterface::Execute_AddToSpellPoints(EffectProperties.SourceCharacter,SpellPointsReward);
				
				bTopOffHealth=true;
				bTopOffMana=true;
				
				IPlayerInterface::Execute_LevelUp(EffectProperties.SourceCharacter);
			}
			
			IPlayerInterface::Execute_AddToXP(EffectProperties.SourceCharacter,LocalXP);
		}
	}
}

void UGAST_AttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute==GetMaxHealthAttribute() && bTopOffHealth)
	{
		SetHealth(GetMaxHealth());
		bTopOffHealth=false;
	}
	if (Attribute==GetMaxManaAttribute() && bTopOffMana)
	{
		SetMana(GetMaxMana());
		bTopOffMana=false;
	}
}

void UGAST_AttributeSet::ShowFloatingText(const FEffectProperties& Props, float Damage, bool bIsBlockedHit,
                                          bool bIsCriticalHit)
{
	if (!IsValid(Props.SourceCharacter) || !IsValid(Props.TargetCharacter)) return;
	if (Props.SourceCharacter!=Props.TargetCharacter)
	{
		if (AGAST_PlayerCOntroller*PC= Cast<AGAST_PlayerCOntroller>(Props.SourceCharacter->Controller))
		{
			PC->ShowFloatingText(Damage,Props.TargetCharacter,bIsBlockedHit,bIsCriticalHit);
			return;
		}
		if (AGAST_PlayerCOntroller*PC= Cast<AGAST_PlayerCOntroller>(Props.TargetCharacter->Controller))
		{
			PC->ShowFloatingText(Damage,Props.TargetCharacter,bIsBlockedHit,bIsCriticalHit);
		}
	}
}

void UGAST_AttributeSet::SendXPReward(const FEffectProperties& Props)
{
	if (Props.TargetCharacter->Implements<UCombatInterface>())
	{
		int32 TargetLevel=ICombatInterface::Execute_GetPlayerLevel(Props.TargetCharacter);
		ECharacterClass TargetClass=ICombatInterface::Execute_GetCharacterClass(Props.TargetCharacter);
		int32 XPReward=UGAST_AbilitySystemLibrary::GetXPByClassAndLevel(Props.TargetCharacter,TargetClass,TargetLevel);

		FGameplayTags GameplayTags= FGameplayTags::Get();
		FGameplayEventData Payload;
		Payload.EventTag=GameplayTags.Attributes_Meta_IncomingXP;
		Payload.EventMagnitude=XPReward;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter,GameplayTags.Attributes_Meta_IncomingXP,Payload);
	}
}

void UGAST_AttributeSet::SetEffectPropertiesByData(const FGameplayEffectModCallbackData& Data,
                                                   FEffectProperties& Props)
{
	Props.GameplayEffectContextHandle=Data.EffectSpec.GetContext();
	Props.SourceASC=Props.GameplayEffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
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
