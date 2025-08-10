// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GAST_AbilitySystemComponent.h"
#include "GameFramework/PlayerController.h"
#include "Input/GAST_InputConfig.h"
#include "UI/Widget/GAST_FloatTextWidgetComponent.h"
#include "GAST_PlayerCOntroller.generated.h"

class IHightLightInterface;
class AMagicCircle;
class UNiagaraSystem;
class USplineComponent;
class IEnemyInterface;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

enum class ETargetingStatus
{
	TargetingEnemy,
	TargetingNonEnemy,
	NoTTargeting
};

/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API AGAST_PlayerCOntroller : public APlayerController
{
	GENERATED_BODY()
public:
	AGAST_PlayerCOntroller();

	UFUNCTION(Client,Reliable)
	void ShowFloatingText(float Damage,ACharacter* TargetCharacter, bool bIsBlockedHit,bool bIsCriticalHit);

	UFUNCTION(BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* MaterialInterface=nullptr);
	UFUNCTION(BlueprintCallable)
	void HideMagicCircle();
	AMagicCircle* GetMagicCircle()const;

	static void HighLightActor(AActor* InActor);
	static void UnHighLightActor(AActor* InActor);

	UFUNCTION(BlueprintImplementableEvent)
	void QuitGame(const FInputActionValue& InputActionValue);
protected:
	/* ----系统自带的虚函数重写---- */
	virtual void PlayerTick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;//这个是绑定输入操作的函数
private:
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputMappingContext> PlayerContext;
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputAction>MoveAction;
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputAction>ShiftAction;
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputAction>QuitAction;
	bool bShiftPressed=false;

	TObjectPtr<AActor> LastActor;
	TObjectPtr<AActor> ThisActor;
	FHitResult UnderCursor;

	void Move(const FInputActionValue& InputActionValue);//移动操作的回调函数
	void ShiftPressed(const FInputActionValue& InputActionValue) {bShiftPressed=true;}
	void ShiftReleased(const FInputActionValue& InputActionValue){bShiftPressed=false;}


	void CursorTrace();//内部给PlayerTick调用的函数，主要用来处理鼠标下面是否是敌人来显示或删除高光

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGAST_FloatTextWidgetComponent>FloatingTextComponent;

	/**
	 * @brief AbilityInput的回调
	 * @param InputTag 用来接收回调发生时传过来的Tag参数
	 */
	void AbilityInputPressed(FGameplayTag InputTag);

	
	/**
	 * @brief AbilityInput的回调
	 * @param InputTag 用来接收回调发生时传过来的Tag参数
	 */
	void AbilityInputHeld(FGameplayTag InputTag);

	
	/**
	 * @brief AbilityInput的回调
	 * @param InputTag 用来接收回调发生时传过来的Tag参数
	 */
	void AbilityInputReleased(FGameplayTag InputTag);
	

	/**
	 * Input配置,将InputAction与InputTag绑定在一起
	 */
	UPROPERTY(EditDefaultsOnly,Category="Input")
	TObjectPtr<UGAST_InputConfig> InputConfig;


	/**
	 * Player的ASC组件变量
	 */
	UPROPERTY()
	TObjectPtr<UGAST_AbilitySystemComponent> PlayerAbilitySystemComponent;

	
	/**
	 * @brief 这个函数是为了减少Cast而创建的，当ASC指针为空时Cast设置，之后直接返回
	 * @return 返回的是自定义的ASC指针
	 */
	UGAST_AbilitySystemComponent* GetASC();

	/**
	 * 实现点击移动
	 */

	FVector CachedDestination=FVector::ZeroVector;//鼠标点击的位置
	float FollowTime=0.f;//鼠标长按的时间
	UPROPERTY(EditDefaultsOnly)
	float ShortPressThread=0.5f;//判定是否是短按的阈值

	//是否在瞄准敌人
	ETargetingStatus TargetingStatus=ETargetingStatus::NoTTargeting;
	bool bAutoRuning=false;//是否在自动行走

	UPROPERTY(EditAnywhere)
	float AutoRunAcceptionRadius=50.f;//允许停止的范围

	TObjectPtr<USplineComponent> SplineComponent;//样条曲线

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ClickNiagaraSystem;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMagicCircle> MagicCircleClass;

	UPROPERTY()
	TObjectPtr<AMagicCircle> MagicCircle;

	void SetMagicCircleLocation();

	void AutoRun();
};
