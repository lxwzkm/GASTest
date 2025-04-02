// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
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

	UPROPERTY(BlueprintReadWrite,meta=(ExposeOnSpawn=true))
	FGameplayEffectSpecHandle DamageSpecHandle;

	/**
	 * @brief 与下面的bHit一起，防止特效和声音因为Destroy先到而Overlap尚未触发而导致的特效声音无法播放而重写destroy函数
	 */
	virtual void Destroyed() override;

protected:
	virtual void BeginPlay() override;

	
	/**
	 * @brief 碰撞的回调函数
	 */
	UFUNCTION()
	void SphereOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult);


private:

	//为了防止网络传播顺序的极端情况，destroy会从服务器复制到客户端，但是如果客户端的Overlap还没有触发，destroy就到了，会直接触发destroy而不会播放特效和声音，
	//所以当我们在客户端收到destroy时，如果还没有播放过特效，就会进入重写的destroy中播放特效和声音
	bool bHit=false;

	float LifeSpan=15.f;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopSound;

	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopSoundComponent;

};
