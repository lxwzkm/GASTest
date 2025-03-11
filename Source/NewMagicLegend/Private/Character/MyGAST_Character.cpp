// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyGAST_Character.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

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