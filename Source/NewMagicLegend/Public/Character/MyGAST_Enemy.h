// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GAST_CharacterBase.h"
#include "Data/CharacterClassInfo.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "MyGAST_Enemy.generated.h"


class AGAST_AIController;
class UBehaviorTree;
class UWidgetComponent;

/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API AMyGAST_Enemy : public AGAST_CharacterBase,public IEnemyInterface
{
	GENERATED_BODY()
public:
	AMyGAST_Enemy();
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	
	/*--------EnemyInterface-------*/
	virtual void HightlightActor() override;//高亮显示的函数
	virtual void UnHighlightActor() override;//取消高亮显示的函数

	/*--------CombatInterface-------*/
	virtual int32 GetPlayerLevel() override;

	virtual void Die() override;
	virtual void Multicast_HandleDie() override;

	UPROPERTY(BlueprintReadOnly,Category="Combat")
	bool bHitReact=false;

	UPROPERTY(BlueprintReadOnly,Category="Walk")
	float BaseWalkSpeed=250.f;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Combat")
	float LifeSpan=5.f;

	void HitReactTagChanged(FGameplayTag CallbackTag,int32 NewCount);

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UWidgetComponent>HealthBar;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangeSignature OnHealthChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangeSignature OnMaxHealthChangedDelegate;
	
protected:
	
	virtual void InitActorInfo() override;//重写父类初始化参数的虚函数
	virtual void InitializeAttributes() override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="EnemyClassDefaults")
	int32 Level=1;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="EnemyClassDefaults")
	ECharacterClass CharacterClass=ECharacterClass::Warrior;

	UPROPERTY(EditAnywhere,Category="AI")
	TObjectPtr<UBehaviorTree>AIBehaviorTree;

	UPROPERTY()
	TObjectPtr<AGAST_AIController>MyAIController;
	
};
