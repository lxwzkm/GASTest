// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GAST_AbilitySystemLibrary.generated.h"

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

	UFUNCTION(BlueprintPure)
	static UOverlayWidgetController* GetOverlayWidgetController(UObject* WordContext);

	UFUNCTION(BlueprintPure)
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(UObject* WordContext);

	/**
	 * 初始化属性（敌人）
	 */
	static void InitializeDefaultsAttributes(const UObject* WordContext,ECharacterClass CharacterClass,float Level,UAbilitySystemComponent*ASC);
	/*
	 * 初始化敌人技能
	 */
	static void InitializeDefaultsAbilities(const UObject*WordContext,UAbilitySystemComponent* ASC);
};
