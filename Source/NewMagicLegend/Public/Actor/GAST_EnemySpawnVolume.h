// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/SavedInterface.h"
#include "GAST_EnemySpawnVolume.generated.h"

class AGAST_EnemySpawnPoint;
class UBoxComponent;

UCLASS()
class NEWMAGICLEGEND_API AGAST_EnemySpawnVolume : public AActor,public ISavedInterface
{
	GENERATED_BODY()
	
public:	
	AGAST_EnemySpawnVolume();

	UPROPERTY(EditAnywhere)
	TArray<AGAST_EnemySpawnPoint*> TargetPoints;

	UPROPERTY(EditAnywhere,SaveGame)
	bool bReached=false;

	/* ------------ SaveInterface -------------*/
	virtual void LoadActor_Implementation() override;
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void BoxOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult);
private:

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxComponent;
};
