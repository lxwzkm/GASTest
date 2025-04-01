// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyGAST_Character.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/GAST_AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Gamemode/GAST_PlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Gamemode/GAST_PlayerCOntroller.h"
#include "UI/HUD/GAST_HUD.h"

AMyGAST_Character::AMyGAST_Character()
{
	GetCharacterMovement()->bOrientRotationToMovement=true;//角色自动将自身的旋转方向调整为移动方向。
	GetCharacterMovement()->bConstrainToPlane=true;//将角色移动束缚在一个平面上
	GetCharacterMovement()->RotationRate=FRotator(0.f,400.f,0.f);//设置角色在Yaw轴的旋转速率
	GetCharacterMovement()->bSnapToPlaneAtStart=true;//角色开始移动时，立即对齐到平面
	
	
	SpringArm=CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetUsingAbsoluteRotation(true);//设置使用绝对旋转
	
	PlayerCamera=CreateDefaultSubobject<UCameraComponent>("PlayerCamera");
	PlayerCamera->SetupAttachment(SpringArm);
	PlayerCamera->bUsePawnControlRotation=false;

	bUseControllerRotationPitch=false;
	bUseControllerRotationRoll=false;
	bUseControllerRotationYaw=false;
	
	
}

void AMyGAST_Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitActorInfo();//初始化角色属性信息
	GiveCharacterAbilites();//初始化角色技能
}

void AMyGAST_Character::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	InitActorInfo();
}

int32 AMyGAST_Character::GetPlayerLevel()
{
	AGAST_PlayerState* AuraPlayerState = GetPlayerState<AGAST_PlayerState>();//GetPlayerState是一个继承下来的模板函数
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerLevel();
}

void AMyGAST_Character::InitActorInfo()
{
	AGAST_PlayerState* AuraPlayerState = GetPlayerState<AGAST_PlayerState>();//GetPlayerState是一个继承下来的模板函数
	check(AuraPlayerState);
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState,this);
	Cast<UGAST_AbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilitySystemComponentSet();//通知ASC初始化已经完成，可以进行委托绑定了
	//顺便设置从父类继承的ASC和AS变量的指针
	AbilitySystemComponent=AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet=AuraPlayerState->GetAttributeSet();

	//初始化UI Overlay的相关参数  PC可以有空的，因为客户端只有自己的Controller，服务器有所有的Controller
	if (AGAST_PlayerCOntroller*PC=Cast<AGAST_PlayerCOntroller>(GetController()))//获取跟Gamemode内一样的Controller才能获取正确的HUD，从而初始化正确的AS和ASC
	{
		if (AGAST_HUD*PlayerHUD=Cast<AGAST_HUD>(PC->GetHUD()))
		{
			PlayerHUD->InitOverlayControllerParams(PC,AuraPlayerState,AttributeSet,AbilitySystemComponent);
		}
	}

	//初始化主要信息，调用父类函数
	InitializeAttributes();
}
