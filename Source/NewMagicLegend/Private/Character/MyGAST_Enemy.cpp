// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyGAST_Enemy.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/GAST_AbilitySystemComponent.h"
#include "AbilitySystem/GAST_AttributeSet.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/GAST_UserWidgetBase.h"

AMyGAST_Enemy::AMyGAST_Enemy()
{
	//实例化ASC组件和属性集
	AbilitySystemComponent=CreateDefaultSubobject<UGAST_AbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);//AIController按照最小模式复制

	AttributeSet=CreateDefaultSubobject<UGAST_AttributeSet>("AttributeSet");

	HealthBar=CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
	
}

void AMyGAST_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
	InitActorInfo();

	if (UGAST_UserWidgetBase* WidgetBase= Cast<UGAST_UserWidgetBase>(HealthBar->GetUserWidgetObject()))
	{
		WidgetBase->SetUserWidgetController(this);
	}

	if (UGAST_AttributeSet*AS= Cast<UGAST_AttributeSet>(AttributeSet))
	{

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddLambda(
[this](const FOnAttributeChangeData& Data){OnHealthChangedDelegate.Broadcast(Data.NewValue);}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute()).AddLambda(
[this](const FOnAttributeChangeData& Data){OnMaxHealthChangedDelegate.Broadcast(Data.NewValue);}
		);
		
		
		OnHealthChangedDelegate.Broadcast(AS->GetHealth());
		OnMaxHealthChangedDelegate.Broadcast(AS->GetMaxHealth());
	}
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
