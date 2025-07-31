// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CheckPoint/CheckPoint.h"
#include "MapEntrance.generated.h"

/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API AMapEntrance : public ACheckPoint
{
	GENERATED_BODY()
public:
	AMapEntrance(const FObjectInitializer& ObjectInitializer);
	/* -----SavedInterface--------- */
	virtual void LoadActor_Implementation() override;


	/* -----------HighLightInterface---------------- */
	virtual void HightlightActor_Implementation() override;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> DestinationMap;

	UPROPERTY(EditAnywhere)
	FName DestinationPlayerStartTag;
protected:
	virtual void SphereOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult)override;
};
