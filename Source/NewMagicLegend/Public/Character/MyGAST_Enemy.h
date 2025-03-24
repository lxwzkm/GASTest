// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GAST_CharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "MyGAST_Enemy.generated.h"

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
	
	/*--------EnemyInterface-------*/
	virtual void HightlightActor() override;//高亮显示的函数
	virtual void UnHighlightActor() override;//取消高亮显示的函数

	/*--------CombatInterface-------*/
	virtual int32 GetPlayerLevel() override;
protected:
	
	virtual void InitActorInfo() override;//重写父类初始化参数的虚函数

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="EnemyClassDefaults")
	int32 Level=1;
	
};
