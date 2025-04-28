// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/GAST_AbilitySystemComponent.h"
#include "GAST_AttributeSet.generated.h"

struct FGameplayEffectModCallbackData;

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties(){}
	
	FGameplayEffectContextHandle GameplayEffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC=nullptr;
	UPROPERTY()
	UAbilitySystemComponent* TargetASC=nullptr;
	UPROPERTY()
	AActor* SourceAvatarActor=nullptr;
	UPROPERTY()
	AActor* TargetAvatarActor=nullptr;
	UPROPERTY()
	AController* SourceController=nullptr;
	UPROPERTY()
	AController* TargetController=nullptr;
	UPROPERTY()
	ACharacter* SourceCharacter=nullptr;
	UPROPERTY()
	ACharacter* TargetCharacter=nullptr;
	
};

DECLARE_DELEGATE_RetVal(FGameplayAttribute,FOnGetAttribtueSignature)

template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

/**
 * 当属性变化时，服务器先通知给客户端，通过GetLifetimeReplicatedProps函数来传递参数，客户端收到新值以后，调用OnRep_Health函数，来执行当属性发生变化以后会执行的动作比如产生伤害数字、血量UI下降、特殊动画特效等操作
 */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
UCLASS()
class NEWMAGICLEGEND_API UGAST_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UGAST_AttributeSet();
	
	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttribute;//用来帮助广播Tag和属性

	/*---------------------  Primary Attributes  --------------------*/
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Strength,Category="Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UGAST_AttributeSet,Strength);
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Intelligence,Category="Primary Attributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UGAST_AttributeSet,Intelligence);
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Resilience,Category="Primary Attributes")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UGAST_AttributeSet,Resilience);
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Vigor,Category="Primary Attributes")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UGAST_AttributeSet,Vigor);

	/*---------------------  Secondary Attributes  --------------------*/
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Armor,Category="Secondary Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UGAST_AttributeSet,Armor);
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_ArmorPenetration,Category="Secondary Attributes")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UGAST_AttributeSet,ArmorPenetration);
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_BlockChance,Category="Secondary Attributes")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UGAST_AttributeSet,BlockChance);
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_CriticalHitChance,Category="Secondary Attributes")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UGAST_AttributeSet,CriticalHitChance);
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_CriticalHitDamage,Category="Secondary Attributes")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UGAST_AttributeSet,CriticalHitDamage);
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_CriticalHitResistance,Category="Secondary Attributes")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UGAST_AttributeSet,CriticalHitResistance);
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_HealthRegeneration,Category="Secondary Attributes")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UGAST_AttributeSet,HealthRegeneration);
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_ManaRegeneration,Category="Secondary Attributes")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UGAST_AttributeSet,ManaRegeneration);
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_MaxHealth,Category="Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGAST_AttributeSet,MaxHealth);
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_MaxMana,Category="Vital Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UGAST_AttributeSet,MaxMana);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_FireResistance,Category="Resitance Attributes")
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UGAST_AttributeSet,FireResistance);
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_LightningResistance,Category="Resitance Attributes")
	FGameplayAttributeData LightningResistance;
	ATTRIBUTE_ACCESSORS(UGAST_AttributeSet,LightningResistance);
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_ArcaneResistance,Category="Resitance Attributes")
	FGameplayAttributeData ArcaneResistance;
	ATTRIBUTE_ACCESSORS(UGAST_AttributeSet,ArcaneResistance);
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_PhysicalResistance,Category="Resitance Attributes")
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UGAST_AttributeSet,PhysicalResistance);
	
	/*---------------------  Vital Attributes  --------------------*/
	//1、设置属性变量  可以复制给客户端
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Health,Category="Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGAST_AttributeSet,Health);//宏里面包含ASC的调用，所以需要包含GAST_AbilitySystemComponent.h的头文件
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Mana,Category="Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UGAST_AttributeSet,Mana);

	/*---------------------  Meta Attributes  --------------------*/
	//占位符，只在服务器帮助计算，不复制到客户端
	UPROPERTY(BlueprintReadOnly,Category="Meta Attribute")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UGAST_AttributeSet,IncomingDamage);

	UPROPERTY(BlueprintReadOnly,Category="Meta Attribute")
	FGameplayAttributeData InComingXP;
	ATTRIBUTE_ACCESSORS(UGAST_AttributeSet,InComingXP);
	
	//3、书写通知ASC的调用函数  当服务器更改数据时,通知客户端
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth);//传递旧值是因为此时客户端已经是新值了，新旧对比可以执行更多操作
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana);


	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength);
	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence);
	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience);
	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor);

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor);
	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration);
	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance);
	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance);
	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage);
	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance);
	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration);
	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration);
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana);

	UFUNCTION()
	void OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance);
	UFUNCTION()
	void OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance);
	UFUNCTION()
	void OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance);
	UFUNCTION()
	void OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance);
	

	//2、重写控制LifeTime的函数,控制那些变量可以在服务器和客户端之间同步，任何有需要从服务器赋值到客户端的变量都要重写此函数
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//父类虚函数，在属性应用前调用，主要用来限制属性
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
private:
	void SetEffectPropertiesByData(const FGameplayEffectModCallbackData& Data,FEffectProperties& Props);
	void ShowFloatingText(const FEffectProperties& Props,float Damage,bool bIsBlockedHit,bool bIsCriticalHit);
	void SendXPReward(const FEffectProperties& Props);
};
