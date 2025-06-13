// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Damage_FireBlast.h"

#include "GAST_AbilitySystemLibrary.h"
#include "Actor/Projectile_FIreBall.h"

FString UDamage_FireBlast::GetCurrentLevelDescription(int32 Level)
{
	const float Damage = DamageValue.GetValueAtLevel(Level);
	const float Cost=GetManaCost(Level);
	const float CD=GetCooldown(Level);
	FString Description=FString::Printf(TEXT(
	"<Title>火球冲击</>\n\n"
	"<Default>当前等级： </><Level>%d</>\n"
	"<Default>当前法力消耗:</><ManaCost> %.1f </>\n"
	"<Default>当前CD:</><Cooldown> %.1f </>\n\n"
	"<Default>发射 %d 个火球并回弹，"
	"造成火系伤害</><Damage> %.1f </>\n"
	"<Default>火系伤害有一定几率造成燃烧效果</>"
	),Level,FMath::Abs(Cost),CD,MaxSpawnNum,Damage);
	return Description;
}

FString UDamage_FireBlast::GetNextLevelDescription(int32 Level)
{
	const float Damage = DamageValue.GetValueAtLevel(Level);
	const float Cost=GetManaCost(Level);
	const float CD=GetCooldown(Level);
	FString Description=FString::Printf(TEXT(
	"<Title>火球冲击</>\n\n"
	"<Default>下一等级： </><Level>%d</>\n"
	"<Default>当前法力消耗:</><ManaCost> %.1f </>\n"
	"<Default>当前CD:</><Cooldown> %.1f </>\n\n"
	"<Default>发射 %d 个火球并回弹，"
	"造成火系伤害</><Damage> %.1f </>\n"
	"<Default>火系伤害有一定几率造成燃烧效果</>"
	),Level,FMath::Abs(Cost),CD,MaxSpawnNum,Damage);
	return Description;
}

TArray<AProjectile_FIreBall*> UDamage_FireBlast::SpawnProjectile_FIreBalls()
{
	TArray<AProjectile_FIreBall*> FireBalls;
	FVector Forward=GetAvatarActorFromActorInfo()->GetActorForwardVector();
	TArray<FRotator> Rotators=UGAST_AbilitySystemLibrary::EvenlySpaceRotators(Forward,FVector::UpVector,360.f,MaxSpawnNum);
	for (const FRotator& Rotator:Rotators)
	{
		FTransform Transform;
		Transform.SetLocation(GetAvatarActorFromActorInfo()->GetActorLocation());
		Transform.SetRotation(Rotator.Quaternion());
		AProjectile_FIreBall* FIreBall= GetWorld()->SpawnActorDeferred<AProjectile_FIreBall>(FireBallClass,Transform,GetOwningActorFromActorInfo(),CurrentActorInfo->PlayerController->GetPawn(),ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		FIreBall->DamageEffectParams=MakeDamageParams();
		FIreBall->ActorToReturn=GetAvatarActorFromActorInfo();
		FIreBall->SetOwner(GetAvatarActorFromActorInfo());
		FIreBall->DamageEffectParam=MakeDamageParams();
		FireBalls.Add(FIreBall);
		FIreBall->FinishSpawning(Transform);
	}
	return FireBalls;
}