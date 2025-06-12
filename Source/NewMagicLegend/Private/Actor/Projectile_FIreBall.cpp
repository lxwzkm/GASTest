// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Projectile_FIreBall.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GAST_AbilitySystemLibrary.h"

void AProjectile_FIreBall::BeginPlay()
{
	Super::BeginPlay();
	StartOutgoingTimeline();
}

void AProjectile_FIreBall::SphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValiedOverlap(OtherActor))return;
	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			FVector DeathImpulse=GetActorForwardVector()*DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse=DeathImpulse;
			DamageEffectParams.TargetASC=TargetASC;
			UGAST_AbilitySystemLibrary::ApplyDamageEffectToTarget(DamageEffectParams);
		}
		
	}
}
