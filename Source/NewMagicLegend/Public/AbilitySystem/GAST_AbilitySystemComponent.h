// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "GAST_AbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FAllAssetTagsContainer,const FGameplayTagContainer&/*GE所属的全部Tag*/)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStartupAbilitiesGiven,UGAST_AbilitySystemComponent*)
DECLARE_DELEGATE_OneParam(FForEachAbility,const FGameplayAbilitySpec&)
/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API UGAST_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	/**
	 * @brief 当此函数被调用时，代表ASC已经初始化完成，可以进行回调函数的绑定了
	 */
	UFUNCTION()
	void AbilitySystemComponentSet();

	//用来向WidgetController广播所有应用的Effect的Tag
	FAllAssetTagsContainer AllAssetTagsContainerDelegate;
	FOnStartupAbilitiesGiven OnStartupAbilitiesGivenDelegate;
	
	bool bGivenAbility=false;

	/**
     * @brief 遍历所有已经激活的函数，将他们广播给OverlayController,由OverlayWidgetController创建Delegate并调用函数
     * @param 
	 */
	void ForEachAbility(const FForEachAbility& Delegate);

	static FGameplayTag GetGameplayTagByAbilitySpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagByAbilitySpec(const FGameplayAbilitySpec& AbilitySpec);
	
	/**
	 * @brief 这个函数的作用是初始化GA
	 * @param StartupAbility 初始化能力数组，在蓝图中设置
	 */
	void GiveCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbility);
	
	/**
	 * @brief 这个函数是给Controller调用的用来根据InputTag激活GA的
	 * @param InputTag 输入标签
	 */
	void AbilityInputHeld(const FGameplayTag& InputTag);
	/**
	 * @brief 这个函数是给Controller调用的用来根据InputTag激活GA的
	 * @param InputTag 输入标签
	 */
	void AbilityInputReleased(const FGameplayTag& InputTag);
protected:
	/**
	 * @brief 回调函数，当GE被应用的时候触发，声明在AbilitySystemComponent.h中
	 * @param AbilitySystemComponent ASC组件
	 * @param EffectSpec 需要应用的效果Spec
	 * @param ActiveEffectHandle 需要应用的效果Handle
	 */
	UFUNCTION(Client,Reliable)
	void Client_AppliedGameplayEffect(UAbilitySystemComponent*AbilitySystemComponent, const FGameplayEffectSpec&EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);

	//因为激活技能只有服务器调用，重写激活的复制的回到函数，好让广播信息可以发送到客户端
	virtual void OnRep_ActivateAbilities() override;
};

