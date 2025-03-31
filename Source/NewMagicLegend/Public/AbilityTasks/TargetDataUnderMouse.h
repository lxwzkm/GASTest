// Fill out your copyright notice in the Description page of Project Settings.
//使用TargetData需要在一开始的时候进行初始化，本次初始化在GAST_AssetManager.cpp
#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMouseLocationSignature,const FGameplayAbilityTargetDataHandle&,DataHandle);

/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable,Category="Ability",meta=(HidePin="OwingAbility",DefaultToSelf="OwningAbility",BlueprintInternalUseOnly="true"))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FOnMouseLocationSignature ValiedData;

private:
	virtual void Activate() override;

	void SendMouseCursor();

	void OnTargetDataReplicatedCallBacks(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
};
