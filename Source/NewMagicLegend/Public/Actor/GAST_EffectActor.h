// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GAST_EffectActor.generated.h"

class UGameplayEffect;

UENUM(BlueprintType)//应用效果的策略枚举
enum class EApplyEffectPolicy
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)//移除效果的策略类型枚举
enum class ERemovalEffectPolicy
{
	RemovalEffect,
	DoNotRemove
};

UCLASS()
class NEWMAGICLEGEND_API AGAST_EffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AGAST_EffectActor();

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor,TSubclassOf<UGameplayEffect> Effect);//这个函数是用来将Actor的GE用在触发重叠的人身上的
protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)//根据效果应用策略来判断在重叠时是否应用效果
	void OnOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)//根据效果应用策略来判断在离开时是否应用效果
	void OnEndOverlap(AActor*TargetActor);

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="ApplyEffect")
	TSubclassOf<UGameplayEffect> InstantGameplayEffect;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="ApplyEffect")
	EApplyEffectPolicy InstantApplyEffectPolicy=EApplyEffectPolicy::DoNotApply;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="ApplyEffect")
	TSubclassOf<UGameplayEffect> DurationGameplayEffect;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="ApplyEffect")
	EApplyEffectPolicy DurationApplyEffectPolicy=EApplyEffectPolicy::DoNotApply;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="ApplyEffect")
	TSubclassOf<UGameplayEffect> InfinitGameplayEffect;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="ApplyEffect")
	EApplyEffectPolicy InfinitApplyEffectPolicy=EApplyEffectPolicy::DoNotApply;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="ApplyEffect")
	ERemovalEffectPolicy InfinitRemoveEffectPolicy=ERemovalEffectPolicy::RemovalEffect;

};
