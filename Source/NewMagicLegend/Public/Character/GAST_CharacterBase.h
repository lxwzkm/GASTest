// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "GAST_CharacterBase.generated.h"

class UGameplayAbility;
class UAttributeSet;
class UAbilitySystemComponent;
class UGameplayEffect;

UCLASS(Abstract)//表明该类是一个抽象类
class NEWMAGICLEGEND_API AGAST_CharacterBase : public ACharacter,public IAbilitySystemInterface,public ICombatInterface
{
	GENERATED_BODY()

public:
	AGAST_CharacterBase();
	/*-------IAbilitySystemInterface-------*/
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet()const;
	virtual FVector GetWeaponSocketLocation() override;
	/*-------CombatInterface-------*/
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die() override;//处理角色死亡事件，只在服务器调用

	UFUNCTION(NetMulticast,Reliable)//客户端服务器均调用，处理死亡事件
	virtual void Multicast_HandleDie();
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;//给玩家和敌人装备的武器的骨骼体组件

	UPROPERTY(EditAnywhere,Category="Combat")
	FName WeaponSocketName;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;//ASC组件敌人在身上实现，玩家在PlayerState上实现

	UPROPERTY()
	TObjectPtr<UAttributeSet>AttributeSet;//属性集，敌人在本身上实现，玩家在PlayerSate上实现
	
	virtual void InitActorInfo();//初始化所有信息
	
	//主要属性类
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect>PrimaryAttributes;

	//次要属性
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect>SecondaryAttributes;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect>InitializeAttribute;//用来初始化属性

	//将属性效果应用在自己身上
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect>GameplayEffect,int level);
	//初始化属性
	virtual void InitializeAttributes();
	//激活角色技能
	void GiveCharacterAbilites();

	/**
	 * Dissove效果
	 */

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Dissove")
	TObjectPtr<UMaterialInstance>DissoveMaterialInstance;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Dissove")
	TObjectPtr<UMaterialInstance>WeaponDissoveMaterialInstance;

	void Dissove();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissove(UMaterialInstanceDynamic* DynamicMaterila);
	UFUNCTION(BlueprintImplementableEvent)
	void WeaponStartDissove(UMaterialInstanceDynamic* DynamicMaterila);

private:

	UPROPERTY(EditAnywhere,Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;//用来保存初始技能

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage>HitReactMontage;
};
