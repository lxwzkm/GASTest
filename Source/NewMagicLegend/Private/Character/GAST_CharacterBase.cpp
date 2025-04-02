// Fill out your copyright notice in the Description page of Project Settings.



#include "Character/GAST_CharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/GAST_AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "NewMagicLegend/NewMagicLegend.h"

AGAST_CharacterBase::AGAST_CharacterBase()
{
 
	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile,ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

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

FVector AGAST_CharacterBase::GetWeaponSocketLocation()
{
	return Weapon->GetSocketLocation(WeaponSocketName);
}

void AGAST_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGAST_CharacterBase::InitActorInfo()
{
	
}

void AGAST_CharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffect, int level)
{
	check(GetAbilitySystemComponent());
	check(GameplayEffect);
	FGameplayEffectContextHandle EffectContextHandle= GetAbilitySystemComponent()->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);//设置效果的发出对象就是本身，主要是应用属性在自身身上
	const FGameplayEffectSpecHandle SpecHandle= GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffect,level,EffectContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),GetAbilitySystemComponent());
}

void AGAST_CharacterBase::InitializeAttributes()
{
	ApplyEffectToSelf(PrimaryAttributes,GetPlayerLevel());
	ApplyEffectToSelf(SecondaryAttributes,GetPlayerLevel());
	ApplyEffectToSelf(InitializeAttribute,GetPlayerLevel());
}

void AGAST_CharacterBase::GiveCharacterAbilites()
{
	if (!HasAuthority())return;

	UGAST_AbilitySystemComponent*ASC=CastChecked<UGAST_AbilitySystemComponent>(AbilitySystemComponent);
	ASC->GiveCharacterAbilities(StartupAbilities);
}

