// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GAST_EffectActor.generated.h"

class USphereComponent;

UCLASS()
class NEWMAGICLEGEND_API AGAST_EffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AGAST_EffectActor();

protected:

	virtual void BeginPlay() override;

public:	

	//球形碰撞的回调函数，声明自带文件写出
	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent>StaticMesh;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent>Sphere;

};
