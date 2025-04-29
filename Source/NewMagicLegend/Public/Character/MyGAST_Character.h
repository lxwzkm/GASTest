// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GAST_CharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "MyGAST_Character.generated.h"

class UGameplayEffect;
class UCameraComponent;
class USpringArmComponent;
/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API AMyGAST_Character : public AGAST_CharacterBase,public IPlayerInterface
{
	GENERATED_BODY()
public:
	AMyGAST_Character();
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<USpringArmComponent>SpringArm;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UCameraComponent>PlayerCamera;
protected:
	//在服务器上运行的函数，主要是为了服务器上初始化ASC的Actor信息
	virtual void PossessedBy(AController* NewController) override;
	//在客户端调用的函数，为了在客户端初始化ASC的Actor信息
	virtual void OnRep_PlayerState() override;

	/*---------------PlayerInterface-----------------*/
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void LevelUp_Implementation() override;
	virtual int32 GetXP_Implementation() override;
	virtual int32 FindLevelForXP_Implementation(int32 InXP) override;

	virtual int32 GetAttributePointsReward_Implementation(int32 Level) override;
	virtual int32 GetSpellPointsReward_Implementation(int32 Level) override;
	virtual void AddToLevel_Implementation(int32 InLevel) override;
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;
	/*---------------CombatIntergace-----------------*/
	virtual int32 GetPlayerLevel_Implementation() override;

private:
	virtual void InitActorInfo() override;
};
