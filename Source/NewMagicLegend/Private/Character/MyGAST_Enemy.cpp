// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyGAST_Enemy.h"

#include "AbilitySystemComponent.h"
#include "GAST_AbilitySystemLibrary.h"
#include "AbilitySystem/GAST_AbilitySystemComponent.h"
#include "AbilitySystem/GAST_AttributeSet.h"
#include "AI/GAST_AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayTag/GAST_GameplayTags.h"
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

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	
}

void AMyGAST_Enemy::BeginPlay()
{
	Super::BeginPlay();

	InitActorInfo();
	if (HasAuthority())
	{
		UGAST_AbilitySystemLibrary::InitializeDefaultsAbilities(this,AbilitySystemComponent,CharacterClass);
	}

	GetCharacterMovement()->MaxWalkSpeed=BaseWalkSpeed;

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

		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTags::Get().Effect_HitReact,EGameplayTagEventType::NewOrRemoved).AddUObject(this,&AMyGAST_Enemy::HitReactTagChanged);
		
		OnHealthChangedDelegate.Broadcast(AS->GetHealth());
		OnMaxHealthChangedDelegate.Broadcast(AS->GetMaxHealth());
	}
}

void AMyGAST_Enemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority())return;
	MyAIController=Cast<AGAST_AIController>(NewController);
	MyAIController->GetBlackboardComponent()->InitializeBlackboard(*AIBehaviorTree->BlackboardAsset);
	MyAIController->RunBehaviorTree(AIBehaviorTree);
	MyAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsHitReacting"),false);
	MyAIController->GetBlackboardComponent()->SetValueAsBool(FName("TargetIsDead"),false);
	MyAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsRanger"),CharacterClass!=ECharacterClass::Warrior);
}

void AMyGAST_Enemy::HitReactTagChanged(FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReact=NewCount>0.f;
	GetCharacterMovement()->MaxWalkSpeed=bHitReact?0.f:BaseWalkSpeed;
	if (MyAIController&&MyAIController->GetBlackboardComponent())
	{
		MyAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsHitReacting"),bHitReact);
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

void AMyGAST_Enemy::SetCombatTarget_Implementation(AActor* Target)
{
	CombatTarget=Target;
}

AActor* AMyGAST_Enemy::GetCombatTarget_Implementation()
{
	return CombatTarget;
}

int32 AMyGAST_Enemy::GetPlayerLevel_Implementation()
{
	return Level;
}

void AMyGAST_Enemy::Die()
{
	SetLifeSpan(LifeSpan);
	if (HealthBar)
	{
		HealthBar->SetVisibility(false);
	}
	if (MyAIController&&MyAIController->GetBlackboardComponent())
	{
		MyAIController->GetBlackboardComponent()->SetValueAsBool(FName("AmIDead"),true);
	}
	Super::Die();
}

void AMyGAST_Enemy::Multicast_HandleDie()
{
	if (HealthBar)
	{
		HealthBar->SetVisibility(false);
	}
	Super::Multicast_HandleDie();
}

void AMyGAST_Enemy::InitActorInfo()
{
	//初始化ASC的OwnerActor和AvatarActor  客户端服务器都调用
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	Cast<UGAST_AbilitySystemComponent>(AbilitySystemComponent)->AbilitySystemComponentSet();//通知已经初始化完成，可以进行委托绑定了

	if (HasAuthority())
	{
		InitializeAttributes();
	}
	
}

void AMyGAST_Enemy::InitializeAttributes()
{
	UGAST_AbilitySystemLibrary::InitializeDefaultsAttributes(this,CharacterClass,Level,AbilitySystemComponent);
}
