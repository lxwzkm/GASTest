// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Projectile_FireBolt.h"
#include "GameplayTag/GAST_GameplayTags.h"
#include "Kismet/KismetSystemLibrary.h"

FString UProjectile_FireBolt::GetCurrentLevelDescription(int32 Level)
{
	const float Damage = DamageValue.GetValueAtLevel(Level);
	const float Cost=GetManaCost(Level);
	const float CD=GetCooldown(Level);
	FString Description=FString::Printf(TEXT(
		"<Title>火球术</>\n\n"
		"<Default>当前等级： </><Level>%d</>\n"
		"<Default>当前法力消耗:</><ManaCost> %.1f </>\n"
		"<Default>当前CD:</><Cooldown> %.1f </>\n\n"
		"<Default>发射 %d 个火球，"
		"造成火系伤害</><Damage> %.1f </>\n"
		"<Default>火系伤害有一定几率造成燃烧效果</>"
		),Level,FMath::Abs(Cost),CD,Level,Damage);
	return Description;
}

FString UProjectile_FireBolt::GetNextLevelDescription(int32 Level)
{
	const float Damage = DamageValue.GetValueAtLevel(Level);
	const float Cost=GetManaCost(Level);
	const float CD=GetCooldown(Level);
	FString Description=FString::Printf(TEXT(
		"<Title>火球术</>\n\n"
		"<Default>下一等级： </><Level>%d</>\n"
		"<Default>法力消耗:</><ManaCost> %.1f </>\n"
		"<Default>CD:</><Cooldown> %.1f </>\n\n"
		"<Default>发射 %d 个火球，"
		"造成火系伤害</><Damage> %.1f </>\n"
		"<Default>火系伤害有一定几率造成燃烧效果</>"
		),Level,FMath::Abs(Cost),CD,FMath::Clamp(Level,1,3),Damage);
	return Description;
}

void UProjectile_FireBolt::SpawnPrijectiles(const FVector& TargetLocation, const FGameplayTag& CombatSocketTag,bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	bool IsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!IsServer)return;
	
	const FVector SocketLocation=ICombatInterface::Execute_GetWeaponSocketLocation(GetAvatarActorFromActorInfo(),CombatSocketTag);
	
	FRotator Rotation=(TargetLocation-SocketLocation).Rotation();
	if (bOverridePitch)
	{
		Rotation.Pitch=PitchOverride;
	}

	const FVector ForWard=Rotation.Vector();
	const FVector LeftSpread=ForWard.RotateAngleAxis(-ProjectileSpread/2.f,FVector::UpVector);
	const FVector RightSpread=ForWard.RotateAngleAxis(ProjectileSpread/2.f,FVector::UpVector);
	
	//int32 ProjectileNum=FMath::Min(MaxSpawnNum,GetAbilityLevel());
	if (ProjectileNum>1)
	{
		const float DeltaSpread=ProjectileSpread/(ProjectileNum-1);//将数量减一是为了发射物的散布，一个的时候不需要散布，两个的时候九十度一边一个，三个的时候，一个45度的间隔，刚好与数量相差一个
		for (int32 i=0;i<ProjectileNum;i++)
		{
			const FVector Direction=LeftSpread.RotateAngleAxis(DeltaSpread*i,FVector::UpVector);
			UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),SocketLocation+FVector(0,0,20),SocketLocation+Direction*100.f,1.f,FColor::Green,120.f,1.f);
		}
	}
	else
	{
		const FVector Direction=ForWard;
		UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),SocketLocation+FVector(0,0,20),SocketLocation+Direction*100.f,1.f,FColor::Green,120.f,1.f);

	}
	
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),SocketLocation,SocketLocation+ForWard*100.f,1.f,FColor::Red,120.f,1.f);
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),SocketLocation,SocketLocation+LeftSpread*100.f,1.f,FColor::Red,120.f,1.f);
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),SocketLocation,SocketLocation+RightSpread*100.f,1.f,FColor::Red,120.f,1.f);	
}
