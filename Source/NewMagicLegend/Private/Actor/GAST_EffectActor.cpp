// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/GAST_EffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

AGAST_EffectActor::AGAST_EffectActor()
{
 
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("RootScene"));
}

void AGAST_EffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> Effect)
{//1 获取目标人物的ASC  2 让目标人物的ASC将效果应用在自己身上
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC==nullptr)return;

	check(Effect);
	FGameplayEffectContextHandle GameplayEffectContextHandle= TargetASC->MakeEffectContext();
	GameplayEffectContextHandle.AddSourceObject(this);//设置效果的发出对象，就是这个Actor本身
	FGameplayEffectSpecHandle GameplayEffectSpecHandle= TargetASC->MakeOutgoingSpec(Effect,1.f,GameplayEffectContextHandle);
	TargetASC->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpecHandle.Data.Get());//因为是TargetASC调用，所以是ToSelf
}


void AGAST_EffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGAST_EffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantApplyEffectPolicy==EApplyEffectPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor,InstantGameplayEffect);
	}
	if (DurationApplyEffectPolicy==EApplyEffectPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor,DurationGameplayEffect);
	}
}

void AGAST_EffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantApplyEffectPolicy==EApplyEffectPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor,InstantGameplayEffect);
	}
	if (InstantApplyEffectPolicy==EApplyEffectPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor,InstantGameplayEffect);
	}
}



