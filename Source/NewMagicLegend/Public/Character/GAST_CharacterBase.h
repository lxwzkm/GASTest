// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "GAST_CharacterBase.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
class UGameplayEffect;

UCLASS(Abstract)//表明该类是一个抽象类
class NEWMAGICLEGEND_API AGAST_CharacterBase : public ACharacter,public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGAST_CharacterBase();
	/*-------IAbilitySystemInterface-------*/
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	/*-------IAbilitySystemInterface  end-------*/
	UAttributeSet* GetAttributeSet()const;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;//给玩家和敌人装备的武器的骨骼体组件

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;//ASC组件敌人在身上实现，玩家在PlayerState上实现

	UPROPERTY()
	TObjectPtr<UAttributeSet>AttributeSet;//属性集，敌人在本身上实现，玩家在PlayerSate上实现
	
	virtual void InitActorInfo();//初始化所有信息
	
	//主要属性类
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect>PrimaryAttributes;

	//次要熟悉
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect>SecondaryAttributes;

	//初始化主要属性
	void InitializePrimaryAttribute() const;
	//初始化次要属性
	void InitializeSecondaryAttribute()const;
};
