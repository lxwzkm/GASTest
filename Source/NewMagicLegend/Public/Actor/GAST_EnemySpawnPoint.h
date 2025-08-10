// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterClassInfo.h"
#include "Engine/TargetPoint.h"
#include "GAST_EnemySpawnPoint.generated.h"

class AMyGAST_Enemy;
/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API AGAST_EnemySpawnPoint : public ATargetPoint
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnPoint)
	TSubclassOf<AMyGAST_Enemy> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnPoint)
	int32 EnemyLevel=1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnPoint)
	ECharacterClass CharacterClass=ECharacterClass::Warrior;
};
