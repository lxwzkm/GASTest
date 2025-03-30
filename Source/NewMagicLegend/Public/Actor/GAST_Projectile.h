// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GAST_Projectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class NEWMAGICLEGEND_API AGAST_Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AGAST_Projectile();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

protected:
	virtual void BeginPlay() override;

	
	/**
	 * @brief 碰撞的回调函数
	 */
	UFUNCTION()
	void SphereOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult);


private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;

};
