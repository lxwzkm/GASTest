// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/GAST_EnemySpawnPoint.h"

#include "Character/MyGAST_Enemy.h"

void AGAST_EnemySpawnPoint::SpawnEnemy()
{
	FActorSpawnParameters SpawnParameters;//这部分原作者没有调用，不会生效，如果要同样设置，在deferred中填入下面一行的参数就行
	SpawnParameters.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AMyGAST_Enemy* SpawnEnemy=GetWorld()->SpawnActorDeferred<AMyGAST_Enemy>(EnemyClass,GetActorTransform());
	SpawnEnemy->SetCharacterClass(CharacterClass);
	SpawnEnemy->SetLevel(EnemyLevel);
	SpawnEnemy->FinishSpawning(GetActorTransform());
	SpawnEnemy->SpawnDefaultController();//默认不会生成控制器
}
