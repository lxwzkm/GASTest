// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/GAST_Projectile.h"
#include "Projectile_FIreBall.generated.h"

/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API AProjectile_FIreBall : public AGAST_Projectile
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintImplementableEvent)
	void StartOutgoingTimeline();

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> ActorToReturn;
protected:
	virtual void SphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	
};
