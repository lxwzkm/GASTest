// Fill out your copyright notice in the Description page of Project Settings.



#include "Character/GAST_CharacterBase.h"

AGAST_CharacterBase::AGAST_CharacterBase()
{
 
	PrimaryActorTick.bCanEverTick = false;

	/* 武器基类设置 */
	Weapon=CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");//实例化组件
	Weapon->SetupAttachment(GetMesh(),FName("WeaponHandSocket"));//根据插槽名称设置武器的吸附插槽
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);//设置组件的碰撞是否开启
}

UAbilitySystemComponent* AGAST_CharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AGAST_CharacterBase::GetAttributeSet() const
{
	return AttributeSet;
}

void AGAST_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

