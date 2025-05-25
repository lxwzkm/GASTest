// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GAST_AbilitySystemLibrary.generated.h"

struct FGameplayTag;
struct FDamageEffectParams;
class UMyAbilityInfo;
class USpellMenuWidgetController;
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
	/**
	 * @brief 初始化WidgetController以及获取他们
	 * @param WordContext 
	 * @return 
	 */
	UFUNCTION(BlueprintCallable)
	static bool MakeWidgetControllerParams(UObject* WordContext,FWidgetControllerParams& OutWCParams,AGAST_HUD*& OutHUD);
	
	UFUNCTION(BlueprintPure,Category="UI",meta=(DefaultToSelf="WordContext"))
	static UOverlayWidgetController* GetOverlayWidgetController(UObject* WordContext);

	UFUNCTION(BlueprintPure,Category="UI",meta=(DefaultToSelf="WordContext"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(UObject* WordContext);

	UFUNCTION(BlueprintPure,Category="UI",meta=(DefaultToSelf="WordContext"))
	static  USpellMenuWidgetController* GetSpellMenuWidgetController(UObject* WordContext);

	/**
	 * 初始化属性（敌人）
	 */
	static void InitializeDefaultsAttributes(const UObject* WordContext,ECharacterClass CharacterClass,float Level,UAbilitySystemComponent*ASC);
	/*
	 * 初始化敌人技能
	 */
	static void InitializeDefaultsAbilities(const UObject*WordContext,UAbilitySystemComponent* ASC,ECharacterClass CharacterClass);

	/*
	 * 获取CharacterClassInfo
	 */
	UFUNCTION(BlueprintCallable,Category="Character")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WordContext);

	UFUNCTION(BlueprintCallable,Category="Character")
	static UMyAbilityInfo* GetAbilityInfo(const UObject* WordContext);
	UFUNCTION(BlueprintCallable,Category="Character")
	static UMyAbilityInfo* GetAbilityInfoFromPlayerState(const UObject* WordContext);

	/**
	 * 处理自定义的GameplayEffectContext事件
	 */
	UFUNCTION(BlueprintPure,Category="Context")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& GameplayEffectContextHandle);
	UFUNCTION(BlueprintPure,Category="Context")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& GameplayEffectContextHandle);
	UFUNCTION(BlueprintPure,Category="Context")
	static bool IsSuccessfulDebuff(const FGameplayEffectContextHandle& GameplayEffectContextHandle);
	UFUNCTION(BlueprintPure,Category="Context")
	static float GetDebuffDamage(const FGameplayEffectContextHandle& GameplayEffectContextHandle);
	UFUNCTION(BlueprintPure,Category="Context")
	static float GetDebuffDuration(const FGameplayEffectContextHandle& GameplayEffectContextHandle);
	UFUNCTION(BlueprintPure,Category="Context")
	static float GetDebuffFrequency(const FGameplayEffectContextHandle& GameplayEffectContextHandle);
	UFUNCTION(BlueprintPure,Category="Context")
	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& GameplayEffectContextHandle);
	UFUNCTION(BlueprintPure,Category="Context")
	static FVector GetDeathImpulse(const FGameplayEffectContextHandle& GameplayEffectContextHandle);
	UFUNCTION(BlueprintPure,Category="Context")
	static FVector GetKnockBackForce(const FGameplayEffectContextHandle& GameplayEffectContextHandle);

	UFUNCTION(BlueprintCallable,Category="Context")
	static void SetIsBlockHit(UPARAM(ref) FGameplayEffectContextHandle& GameplayEffectContextHandle,bool bInIsBlocked);
	UFUNCTION(BlueprintCallable,Category="Context")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& GameplayEffectContextHandle,bool bInIsCritical);
	UFUNCTION(BlueprintCallable,Category="Context")
	static void SetIsSuccessfulDebuff(UPARAM(ref) FGameplayEffectContextHandle& GameplayEffectContextHandle,bool bInIsSuccessfulDebuff);
	UFUNCTION(BlueprintCallable,Category="Context")
	static void SetDebuffDamage(UPARAM(ref) FGameplayEffectContextHandle& GameplayEffectContextHandle,float DebuffDamge);
	UFUNCTION(BlueprintCallable,Category="Context")
	static void SetDebuffDuration(UPARAM(ref) FGameplayEffectContextHandle& GameplayEffectContextHandle,float DebuffDuration);
	UFUNCTION(BlueprintCallable,Category="Context")
	static void SetDebuffFrequency(UPARAM(ref) FGameplayEffectContextHandle& GameplayEffectContextHandle,float DebuffFrequency);
	UFUNCTION(BlueprintCallable,Category="Context")
	static void SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& GameplayEffectContextHandle,const FGameplayTag& DamageType);
	UFUNCTION(BlueprintCallable,Category="Context")
	static void SetDeathImpulse(UPARAM(ref) FGameplayEffectContextHandle& GameplayEffectContextHandle,const FVector& InDeathImpulse);
	UFUNCTION(BlueprintCallable,Category="Context")
	static void SetKnockBackForce(UPARAM(ref) FGameplayEffectContextHandle& GameplayEffectContextHandle,const FVector& InKnockBackForce);

	UFUNCTION(BlueprintCallable,Category="Context")
	static FGameplayEffectContextHandle ApplyDamageEffectToTarget(const FDamageEffectParams& DamageEffectParams);

	/**
	 * 获取半径内存活的所有玩家
	 * @param WordContext 
	 * @param OutOverlapActors 输出结果，存货的player
	 * @param ActorsToIgnore 需要忽视的actors
	 * @param Radius 检测的半径
	 * @param SphereOrigin 检测球形的圆心坐标
	 */
	UFUNCTION(BlueprintCallable,Category="Combat")
	static void GetLivePlayersWithInRadius(const UObject* WordContext,TArray<AActor*>& OutOverlapActors,const TArray<AActor*>&ActorsToIgnore,float Radius,const FVector& SphereOrigin);

	UFUNCTION(BlueprintCallable,Category="Combat")
	static void GetCloestTargets(int32 MaxTargets,const TArray<AActor*>& Actors,TArray<AActor*>& OutActors,const FVector& Origin);

	UFUNCTION(BlueprintPure,Category="Combat")
	static bool IsNotFriend(AActor* FirstActor,AActor* SecondActor);

	static int32 GetXPByClassAndLevel(const UObject* WordContext,ECharacterClass CharacterClass,int32 Level);

	/**
	 * @brief 计算平均分配的旋转角度
	 * @param Forward 向前的向量
	 * @param Axis围绕哪个轴旋转的向量
	 * @param Spread 分散的总角度
	 * @param NumRatators 分散的数量
	 */
	UFUNCTION(BlueprintPure,Category="Combat")
	static TArray<FRotator> EvenlySpaceRotators(const FVector& Forward,const FVector& Axis,float Spread,int32 NumRatators);
	UFUNCTION(BlueprintPure,Category="Combat")
	static TArray<FVector> EvenlyRatatorVectors(const FVector& Forward,const FVector& Axis,float Spread,int32 NumVectors);

	/**
	 * 异步加载图片
	 */

	UFUNCTION(BlueprintCallable)
	static void AsyncLoadImage(const FString& Path);
};


