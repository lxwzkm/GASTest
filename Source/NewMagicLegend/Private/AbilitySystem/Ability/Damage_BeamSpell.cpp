// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Damage_BeamSpell.h"

#include "GAST_AbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

void UDamage_BeamSpell::StoreMouseHitResult(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		MouseHitLocation = HitResult.ImpactPoint;
		MouseHitActor=HitResult.GetActor();
	}
	else
	{
		//取消能力
		CancelAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true);
	}
}

void UDamage_BeamSpell::StoreOwnerVariables()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController=CurrentActorInfo->PlayerController.Get();
		OwnerCharacter=Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	}
	
}

void UDamage_BeamSpell::TraceFirstActor(const FVector& TargetLocation)
{
	check(OwnerCharacter);

	if (OwnerCharacter->Implements<UCombatInterface>())
	{
		USkeletalMeshComponent* Weapon=ICombatInterface::Execute_GetWeaponComponent(OwnerCharacter);
		FVector WeaponLocation=Weapon->GetSocketLocation(FName("TipSocket"));
		TArray<AActor*>ActorsToIgnore;
		ActorsToIgnore.Add(OwnerCharacter);
		FHitResult HitResult;
		UKismetSystemLibrary::SphereTraceSingle(OwnerCharacter,WeaponLocation,TargetLocation,10.f,
			TraceTypeQuery1,false,ActorsToIgnore,EDrawDebugTrace::None,
			HitResult,true);

		if (HitResult.bBlockingHit)
		{
			MouseHitLocation=HitResult.ImpactPoint;
			MouseHitActor=HitResult.GetActor();
		}
	}
	if (ICombatInterface* CombatInterface=Cast<ICombatInterface>(OwnerCharacter))
	{
		if (!CombatInterface->GetDeathDelegate().IsAlreadyBound(this,&UDamage_BeamSpell::PrimaryActorDeath))
		{
			CombatInterface->GetDeathDelegate().AddDynamic(this,&UDamage_BeamSpell::PrimaryActorDeath);
		}
	}
}

void UDamage_BeamSpell::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets)
{
	TArray<AActor*> LiveCharacters;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	ActorsToIgnore.Add(MouseHitActor);
	UGAST_AbilitySystemLibrary::GetLivePlayersWithInRadius(GetAvatarActorFromActorInfo(),LiveCharacters,ActorsToIgnore,850.f,MouseHitActor->GetActorLocation());

	int32 NumAdditionalTargets = FMath::Min(GetAbilityLevel()-1,MaxNumAdditionalTargets);
	//NumAdditionalTargets=5;

	UGAST_AbilitySystemLibrary::GetCloestTargets(NumAdditionalTargets,LiveCharacters,OutAdditionalTargets,MouseHitActor->GetActorLocation());

	for (AActor* Target:OutAdditionalTargets)
	{
		if (ICombatInterface* CombatInterface=Cast<ICombatInterface>(Target))
		{
			if (!CombatInterface->GetDeathDelegate().IsAlreadyBound(this,&UDamage_BeamSpell::AdditionalActorDeath))
			{
				CombatInterface->GetDeathDelegate().AddDynamic(this,&UDamage_BeamSpell::AdditionalActorDeath);
			}
		}
	}
}
