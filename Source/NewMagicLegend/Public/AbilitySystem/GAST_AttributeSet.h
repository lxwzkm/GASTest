// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystem/GAST_AbilitySystemComponent.h"
#include "GAST_AttributeSet.generated.h"

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

	//1、设置属性变量  可以复制给客户端
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Health,Category="Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGAST_AttributeSet,Health);//宏里面包含ASC的调用，所以需要包含GAST_AbilitySystemComponent.h的头文件
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_MaxHealth,Category="Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGAST_AttributeSet,MaxHealth);
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Mana,Category="Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UGAST_AttributeSet,Mana);
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_MaxMana,Category="Vital Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UGAST_AttributeSet,MaxMana);
	
	//3、书写通知ASC的调用函数  当服务器更改数据时,通知客户端
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth);//传递旧值是因为此时客户端已经是新值了，新旧对比可以执行更多操作
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana);
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana);
	

	//2、重写控制LifeTime的函数,控制那些变量可以在服务器和客户端之间同步，任何有需要从服务器赋值到客户端的变量都要重写此函数
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
