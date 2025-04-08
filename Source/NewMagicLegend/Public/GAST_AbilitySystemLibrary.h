// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GAST_AbilitySystemLibrary.generated.h"

struct FGameplayEffectContextHandle;
class UAbilitySystemComponent;
enum class ECharacterClass;
class UAttributeMenuWidgetController;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API UGAST_AbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintPure,Category="UI")
	static UOverlayWidgetController* GetOverlayWidgetController(UObject* WordContext);

	UFUNCTION(BlueprintPure,Category="UI")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(UObject* WordContext);

	/**
	 * 初始化属性（敌人）
	 */
	static void InitializeDefaultsAttributes(const UObject* WordContext,ECharacterClass CharacterClass,float Level,UAbilitySystemComponent*ASC);
	/*
	 * 初始化敌人技能
	 */
	static void InitializeDefaultsAbilities(const UObject*WordContext,UAbilitySystemComponent* ASC);

	/*
	 * 获取CharacterClassInfo
	 */
	UFUNCTION(BlueprintCallable,Category="Character")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WordContext);

	/**
	 * 处理自定义的GameplayEffectContext事件
	 */
	UFUNCTION(BlueprintPure,Category="Context")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& GameplayEffectContextHandle);
	UFUNCTION(BlueprintPure,Category="Context")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& GameplayEffectContextHandle);

	UFUNCTION(BlueprintCallable,Category="Context")
	static void SetIsBlockHit(UPARAM(ref) FGameplayEffectContextHandle& GameplayEffectContextHandle,bool bInIsBlocked);
	UFUNCTION(BlueprintCallable,Category="Context")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& GameplayEffectContextHandle,bool bInIsCritical);
};
