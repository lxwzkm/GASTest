// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Projectile_FIreBall.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayCueManager.h"
#include "GAST_AbilitySystemLibrary.h"
#include "GameplayTag/GAST_GameplayTags.h"

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

void AProjectile_FIreBall::OnHit()
{
	if (GetOwner())
	{
		FGameplayCueParameters Parameters;
		Parameters.Location= GetActorLocation();
		UGameplayCueManager::ExecuteGameplayCue_NonReplicated(GetOwner(),FGameplayTags::Get().Gameplaycue_FireBlast,Parameters);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "FireBlast");
	}
	bHit=true;
}
