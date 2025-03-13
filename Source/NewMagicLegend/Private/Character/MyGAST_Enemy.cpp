// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyGAST_Enemy.h"

#include "AbilitySystemComponent.h"

AMyGAST_Enemy::AMyGAST_Enemy()
{
	//实例化ASC组件和属性集
	AbilitySystemComponent=CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);//AIController按照最小模式复制

	AttributeSet=CreateDefaultSubobject<UAttributeSet>("AttributeSet");
}

void AMyGAST_Enemy::BeginPlay()
{
	Super::BeginPlay();
	//初始化ASC的OwnerActor和AvatarActor
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	
}

void AMyGAST_Enemy::HightlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->CustomDepthStencilValue=250.f;
	Weapon->SetRenderCustomDepth(true);
	Weapon->CustomDepthStencilValue=250.f;
}

void AMyGAST_Enemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}
