// Fill out your copyright notice in the Description page of Project Settings.



#include "Character/GAST_CharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/GAST_AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameplayTag/GAST_GameplayTags.h"
#include "Kismet/GameplayStatics.h"
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

FVector AGAST_CharacterBase::GetWeaponSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	const FGameplayTags& GameplayTags=FGameplayTags::Get();
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Weapon)&&IsValid(Weapon))
	{
		return Weapon->GetSocketLocation(WeaponSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Tail))
	{
		return GetMesh()->GetSocketLocation(TailSocketName);
	}
	return FVector();
}

UAnimMontage* AGAST_CharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void AGAST_CharacterBase::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld,true));
	Multicast_HandleDie();
}

bool AGAST_CharacterBase::IsDead_Implementation() const
{
	return IsDead;
}

AActor* AGAST_CharacterBase::GetAvatar_Implementation()
{
	return this;
}

TArray<FTagMontage> AGAST_CharacterBase::GetTagMontages_Implementation()
{
	return AttackMontageToTag;
}

int32 AGAST_CharacterBase::GetMinionCount_Implementation()
{
	return MinionCount;
}

void AGAST_CharacterBase::UpdateMinionCount_Implementation(int32 Amount)
{
	MinionCount+=Amount;
}

ECharacterClass AGAST_CharacterBase::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

UNiagaraSystem* AGAST_CharacterBase::GetImpactEffect_Implementation()
{
	return ImpactEffect;
}

FTagMontage AGAST_CharacterBase::GetTagMontageByTag_Implementation(const FGameplayTag& MontageTag)
{
	for (auto& TagMontage:AttackMontageToTag)
	{
		if (TagMontage.MontageTag.MatchesTagExact(MontageTag))
		{
			return TagMontage;
		}
	}
	return FTagMontage();
}

void AGAST_CharacterBase::Multicast_HandleDie_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(this,DeathSound,GetActorLocation(),GetActorRotation());
	
	Weapon->SetEnableGravity(true);
	Weapon->SetSimulatePhysics(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::Type::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissove();
	IsDead=true;
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
	ApplyEffectToSelf(PrimaryAttributes,ICombatInterface::Execute_GetPlayerLevel(this));
	ApplyEffectToSelf(SecondaryAttributes,ICombatInterface::Execute_GetPlayerLevel(this));
	ApplyEffectToSelf(InitializeAttribute,ICombatInterface::Execute_GetPlayerLevel(this));
}

void AGAST_CharacterBase::GiveCharacterAbilites()
{
	if (!HasAuthority())return;

	UGAST_AbilitySystemComponent*ASC=CastChecked<UGAST_AbilitySystemComponent>(AbilitySystemComponent);
	ASC->GiveCharacterAbilities(StartupAbilities);
	ASC->GiveCharacterPassiveAbilities(StartupPassiveAbilities);
}

void AGAST_CharacterBase::Dissove()
{
	if (IsValid(DissoveMaterialInstance))
	{
		UMaterialInstanceDynamic* Dynamicinst= UMaterialInstanceDynamic::Create(DissoveMaterialInstance,this);
		GetMesh()->SetMaterial(0,Dynamicinst);
		StartDissove(Dynamicinst);
	}
	if (IsValid(WeaponDissoveMaterialInstance))
	{
		UMaterialInstanceDynamic* WeaponDynamicinst=UMaterialInstanceDynamic::Create(WeaponDissoveMaterialInstance,this);
		Weapon->SetMaterial(0,WeaponDynamicinst);
		WeaponStartDissove(WeaponDynamicinst);
	}
}

