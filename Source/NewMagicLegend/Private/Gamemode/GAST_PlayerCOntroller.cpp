// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemode/GAST_PlayerCOntroller.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AGAST_PlayerCOntroller::AGAST_PlayerCOntroller()
{
	bReplicates=true;//可以被复制到客户端
}

void AGAST_PlayerCOntroller::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AGAST_PlayerCOntroller::CursorTrace()
{
	FHitResult UnderCursor;
	GetHitResultUnderCursor(ECC_Visibility,false,UnderCursor);

	if (!UnderCursor.bBlockingHit)return;//如果没有遇到阻挡，就返回

	LastActor=ThisActor;
	ThisActor=Cast<IEnemyInterface>(UnderCursor.GetActor());

	if (LastActor==nullptr)
	{
		if (ThisActor!=nullptr)
		{
			ThisActor->HightlightActor();
		}
	}
	else
	{//Last Actor!=null
		if (ThisActor==nullptr)
		{
			LastActor->UnHighlightActor();
		}
		else
		{
			if (LastActor!=ThisActor)
			{
				LastActor->UnHighlightActor();
				ThisActor->HightlightActor();
			}
		}
	}
}

void AGAST_PlayerCOntroller::BeginPlay()
{
	Super::BeginPlay();

	check(PlayerContext);//检查输入映射上下文是否设置
	UEnhancedInputLocalPlayerSubsystem* LocalPlayerSubsystem=ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());//获取本地增强输入子系统

	check(LocalPlayerSubsystem);//检测本地增强子系统是否为空
	LocalPlayerSubsystem->AddMappingContext(PlayerContext,0);//添加增强输入上下文

	bShowMouseCursor=true;//设置显示鼠标
	DefaultMouseCursor=EMouseCursor::Type::Default;//设置鼠标样式为默认样式

	FInputModeGameAndUI InputModeData;//实例化一个可以设置输入模式数据的变量
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);//不将鼠标锁定到视口
	InputModeData.SetHideCursorDuringCapture(false);//在捕获输入时，不隐藏鼠标光标
	SetInputMode(InputModeData);//将前面设置的结果设置为当前使用的模式
}

void AGAST_PlayerCOntroller::SetupInputComponent()
{
	Super::SetupInputComponent();

	//首先获取增强输入组件，InputComponent继承自父类，之后通过组件来绑定输入操作
	UEnhancedInputComponent*EnhancedInputComponent=CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AGAST_PlayerCOntroller::Move);
}

void AGAST_PlayerCOntroller::Move(const FInputActionValue& InputActionValue)
{
	
	const FVector2D InputValue=InputActionValue.Get<FVector2D>();//处理接收到的输入数据
	const FRotator ControllerRotation=GetControlRotation();//获取控制器的旋转
	const FRotator YawRotation(0.f,ControllerRotation.Yaw,0.f);//将控制器的旋转只保留Yaw的旋转包装起来

	const FVector ForwardDirection=FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);//确定向前的向量
	const FVector RightDirection=FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);//确定向右的向量

	if (APawn*ControlledPawn=GetPawn<APawn>())//如果能够获取到控制的角色，就将获取到的向量及玩家输入进来的方向输入进输入系统
	{
		ControlledPawn->AddMovementInput(ForwardDirection,InputValue.Y);
		ControlledPawn->AddMovementInput(RightDirection,InputValue.X);
	}
}

