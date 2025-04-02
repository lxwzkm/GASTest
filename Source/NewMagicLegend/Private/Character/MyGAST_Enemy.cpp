// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyGAST_Enemy.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/GAST_AbilitySystemComponent.h"

AMyGAST_Enemy::AMyGAST_Enemy()
{
	//实例化ASC组件和属性集
	AbilitySystemComponent=CreateDefaultSubobject<UGAST_AbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);//AIController按照最小模式复制

	AttributeSet=CreateDefaultSubobject<UAttributeSet>("AttributeSet");
}

void AMyGAST_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
	InitActorInfo();
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

int32 AMyGAST_Enemy::GetPlayerLevel()
{
	return Level;
}

void AMyGAST_Enemy::InitActorInfo()
{
	//初始化ASC的OwnerActor和AvatarActor
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	Cast<UGAST_AbilitySystemComponent>(AbilitySystemComponent)->AbilitySystemComponentSet();//通知已经初始化完成，可以进行委托绑定了

	InitializeAttributes();

}
