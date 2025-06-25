// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "CheckPoint.generated.h"

class USphereComponent;
/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API ACheckPoint : public APlayerStart
{
	GENERATED_BODY()
public:
	ACheckPoint(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintImplementableEvent)
	void CheckPointReached(UMaterialInstanceDynamic* DynamicMaterial);

	void HandleGlowEffects();
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void SphereOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult);
private:

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> CheckPointMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;

	
};
