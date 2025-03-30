// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemode/GAST_PlayerCOntroller.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Components/SplineComponent.h"
#include "GameplayTag/GAST_GameplayTags.h"
#include "Input/GAST_EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AGAST_PlayerCOntroller::AGAST_PlayerCOntroller()
{
	bReplicates=true;//可以被复制到客户端

	SplineComponent=CreateDefaultSubobject<USplineComponent>("Spline");//实例化样条曲线
}

void AGAST_PlayerCOntroller::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
	AutoRun();
}

void AGAST_PlayerCOntroller::AutoRun()
{
	if (!bAutoRuning)return;
	if (APawn*ControlledPawn=GetPawn())
	{
		const FVector ClosedLocation=SplineComponent->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(),ESplineCoordinateSpace::World);
		const FVector ClosedDirection=SplineComponent->FindDirectionClosestToWorldLocation(ClosedLocation,ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(ClosedDirection);
		
		float Length= (ClosedLocation-CachedDestination).Length();
		if (Length<=AutoRunAcceptionRadius)
		{
			bAutoRuning=false;
		}
	}
}

void AGAST_PlayerCOntroller::CursorTrace()
{
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

void AGAST_PlayerCOntroller::AbilityInputPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FGameplayTags::Get().Input_LMB))
	{
		bTargeting=ThisActor?true:false;//通过ThisActor是否为空来判断bTargeting是否为真
		bAutoRuning=false;//此时还不知道是否是短按，所以设置为false
	}

}

void AGAST_PlayerCOntroller::AbilityInputHeld(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FGameplayTags::Get().Input_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputHeld(InputTag);
		}
		return;
	}
	
	if (bTargeting)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputHeld(InputTag);
		}
	}
	else
	{
		FollowTime+=GetWorld()->GetDeltaSeconds();
		
		if (UnderCursor.bBlockingHit)
		{
			CachedDestination=UnderCursor.ImpactPoint;
			APawn* ControlledPawn=GetPawn();
			FVector Direction=(CachedDestination-ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(Direction);
		}
	}
}

void AGAST_PlayerCOntroller::AbilityInputReleased(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FGameplayTags::Get().Input_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputReleased(InputTag);
		}
		return;
	}
	
	if (bTargeting)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputReleased(InputTag);
		}
	}
	else
	{
		APawn* ControlledPawn=GetPawn();
		if (FollowTime<=ShortPressThread&&ControlledPawn)
		{
			if (UNavigationPath* Path= UNavigationSystemV1::FindPathToLocationSynchronously(this,ControlledPawn->GetActorLocation(),CachedDestination))
			{
				SplineComponent->ClearSplinePoints();
				for (auto& PointsLoc:Path->PathPoints)
				{
					SplineComponent->AddSplinePoint(PointsLoc,ESplineCoordinateSpace::World);
					
				}
				bAutoRuning=true;
			}
		}
		FollowTime=0.f;
		bTargeting=false;
	}
}

UGAST_AbilitySystemComponent* AGAST_PlayerCOntroller::GetASC()
{
	if (PlayerAbilitySystemComponent==nullptr)
	{
		PlayerAbilitySystemComponent=Cast<UGAST_AbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return PlayerAbilitySystemComponent;
}

void AGAST_PlayerCOntroller::BeginPlay()
{
	Super::BeginPlay();
	
	check(PlayerContext);
	if (UEnhancedInputLocalPlayerSubsystem* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		LocalPlayerSubsystem->AddMappingContext(PlayerContext, 0);
	}

	// 设置鼠标光标
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	// 设置输入模式
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);

}

void AGAST_PlayerCOntroller::SetupInputComponent()
{
	Super::SetupInputComponent();

	//首先获取增强输入组件，InputComponent继承自父类，之后通过组件来绑定输入操作
	UGAST_EnhancedInputComponent*EnhancedInputComponent=CastChecked<UGAST_EnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AGAST_PlayerCOntroller::Move);
	//调用GAST_EnhancedInputComponent中的绑定函数，将InputConfig中的输入行为、Tag与回调函数进行绑定
	EnhancedInputComponent->BindAbilityActions(InputConfig,this,&ThisClass::AbilityInputPressed,&ThisClass::AbilityInputHeld,&ThisClass::AbilityInputReleased);
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

