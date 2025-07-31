// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/HightLightInterface.h"
#include "Interaction/SavedInterface.h"
#include "NewMagicLegend/NewMagicLegend.h"
#include "CheckPoint.generated.h"

class USphereComponent;
/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API ACheckPoint : public APlayerStart,public ISavedInterface,public IHightLightInterface
{
	GENERATED_BODY()
public:
	ACheckPoint(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintImplementableEvent)
	void CheckPointReached(UMaterialInstanceDynamic* DynamicMaterial);
	UFUNCTION(BlueprintCallable)
	void HandleGlowEffects();

	/* -----SavedInterface--------- */
	virtual bool ShouldReplaceTransform_Implementation() override {return false;}
	virtual void LoadActor_Implementation() override;


	/* -----------HighLightInterface---------------- */
	virtual void HightlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	virtual void SetMoveToLocation_Implementation(FVector& OutComponentLocation) override;

	UPROPERTY(BlueprintReadWrite,SaveGame)
	bool bCheckPointReached=false;

	UPROPERTY(EditAnywhere,SaveGame)
	bool bBindOverlap=true;
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void SphereOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> CheckPointMesh;

	UPROPERTY(EditDefaultsOnly)
	int32 CustomDepthValue=CUSTOM_DEPTH_TAN;
private:
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> MoveToComponent;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;

	
};
