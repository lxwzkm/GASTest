// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/GAST_EffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AGAST_EffectActor::AGAST_EffectActor()
{
 
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("RootScene"));
}

void AGAST_EffectActor::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeLocation=GetActorLocation();
	CalculatedLocation=GetActorLocation();
	CalculatedRotation=GetActorRotation();
}

void AGAST_EffectActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	RunningTime+=DeltaSeconds;
	float SinPeriod = 2*PI/SinperiodConstant;
	if (RunningTime>SinPeriod)
	{
		RunningTime=0;
	}
	SinMovement(DeltaSeconds);	
}

void AGAST_EffectActor::SinMovement(float DeltaTime)
{
	if (bRotation)
	{
		const FRotator DeltaRotation(0.f,DeltaTime*RotationRate,0.f);
		CalculatedRotation=UKismetMathLibrary::ComposeRotators(CalculatedRotation,DeltaRotation);
	}
	if (bSinMovement)
	{
		const float Sine=SinAmplitude*FMath::Sin(RunningTime*SinperiodConstant);
		CalculatedLocation=InitializeLocation+FVector(0.f,0.f,Sine);
	}
}
void AGAST_EffectActor::StartRotateMovement()
{
	bRotation=true;
	CalculatedRotation=GetActorRotation();
}

void AGAST_EffectActor::StartSinMovement()
{
	bSinMovement=true;
	InitializeLocation=GetActorLocation();
	CalculatedLocation=GetActorLocation();
}


void AGAST_EffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> Effect)
{//1 获取目标人物的ASC  2 让目标人物的ASC将效果应用在自己身上

	if (TargetActor->ActorHasTag("Enemy")&&!bApplyEffectToEnemy)return;
	
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC==nullptr)return;

	check(Effect);
	FGameplayEffectContextHandle GameplayEffectContextHandle= TargetASC->MakeEffectContext();
	GameplayEffectContextHandle.AddSourceObject(this);//设置效果的发出对象，就是这个Actor本身
	FGameplayEffectSpecHandle GameplayEffectSpecHandle= TargetASC->MakeOutgoingSpec(Effect,ActorLevel,GameplayEffectContextHandle);
	FActiveGameplayEffectHandle ActiveEffectHandle= TargetASC->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpecHandle.Data.Get());//因为是TargetASC调用，所以是ToSelf
	//处理Infinite效果，将需要移除的效果放入Map容器内
	const bool bIsInfinite= GameplayEffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy==EGameplayEffectDurationType::Infinite;
	if (bIsInfinite&&InfinitRemoveEffectPolicy==ERemovalEffectPolicy::RemovalEffect)
	{
		ActiveInfiniteEffects.Add(ActiveEffectHandle,TargetASC);
	}
	if (!bIsInfinite)
	{
		Destroy();
	}
}

void AGAST_EffectActor::OnOverlap(AActor* TargetActor)
{
	if (TargetActor->ActorHasTag("Enemy")&&!bApplyEffectToEnemy)return;
	
	if (InstantApplyEffectPolicy==EApplyEffectPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor,InstantGameplayEffect);
	}
	if (DurationApplyEffectPolicy==EApplyEffectPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor,DurationGameplayEffect);
	}
	if (InfinitApplyEffectPolicy==EApplyEffectPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor,InfinitGameplayEffect);
	}
}

void AGAST_EffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (TargetActor->ActorHasTag("Enemy")&&!bApplyEffectToEnemy)return;
	
	if (InstantApplyEffectPolicy==EApplyEffectPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor,InstantGameplayEffect);
	}
	if (InstantApplyEffectPolicy==EApplyEffectPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor,InstantGameplayEffect);
	}
	if (InfinitApplyEffectPolicy==EApplyEffectPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor,InfinitGameplayEffect);
	}
	if (InfinitRemoveEffectPolicy==ERemovalEffectPolicy::RemovalEffect)
	{
		UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

		TArray<FActiveGameplayEffectHandle> RemoveHandle;//用来记录需要移除的效果的数组
		for (TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> PairHandle:ActiveInfiniteEffects)
		{//遍历Map找到需要移除的效果，记录在RemoveHandle数组内
			if (TargetASC==PairHandle.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(PairHandle.Key,1);
				RemoveHandle.Add(PairHandle.Key);
			}
		}
		for (auto& Handle:RemoveHandle)
		{
			ActiveInfiniteEffects.FindAndRemoveChecked(Handle);
		}
	}
}



