// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GAST_AbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FAllAssetTagsContainer,const FGameplayTagContainer&/*GE所属的全部Tag*/)

/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API UGAST_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	//当此函数被调用时，代表ASC已经初始化完成，可以进行回调函数的绑定了
	UFUNCTION()
	void AbilitySystemComponentSet();

	FAllAssetTagsContainer AllAssetTagsContainerDelegate;
protected:
	//回调函数，当GE被应用的时候触发，声明在AbilitySystemComponent.h中
	UFUNCTION()
	void AppliedGameplayEffect(UAbilitySystemComponent*AbilitySystemComponent, const FGameplayEffectSpec&EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
};
