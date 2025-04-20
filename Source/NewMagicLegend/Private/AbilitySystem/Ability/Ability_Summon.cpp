// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Ability_Summon.h"
TArray<FVector> UAbility_Summon::GetSpawnLocations()
{
	const FVector Forward=GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location=GetAvatarActorFromActorInfo()->GetActorLocation();
	//先让向前的向量，旋转到左边界，再计算需要旋转几次来生成几个不同位置的召唤物
	const FVector LeftOfSpread=Forward.RotateAngleAxis(-SpawnSpread/2,FVector::UpVector);
	const float DelateSpread=SpawnSpread/NumMinions;
	TArray<FVector> SpawnLocations;
	for (int32 i=0;i<NumMinions;i++)
	{
		const FVector Direction=LeftOfSpread.RotateAngleAxis(DelateSpread*i,FVector::UpVector);
		FVector RandSpawnLocation=Location+Direction*FMath::FRandRange(MinDistance,MaxDistance);

		//判断斜坡
		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult,RandSpawnLocation+FVector(0.f,0.f,400.f),RandSpawnLocation-FVector(0.f,0.f,400.f),ECC_Visibility);
		if (HitResult.bBlockingHit)
		{
			RandSpawnLocation=HitResult.ImpactPoint;
		}
		
		SpawnLocations.Add(RandSpawnLocation);
		//DrawDebugSphere(GetWorld(),RandSpawnLocation,4.f,8.f,FColor::Cyan,false,4.f);
		//UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),Location,Location+Direction*MaxDistance,4,FColor::Blue,4.f);
	}
	return SpawnLocations;
}

TSubclassOf<APawn> UAbility_Summon::GetRandomMinionClass()
{
	int32 Selection=FMath::RandRange(0,MinionClasses.Num()-1);
	return MinionClasses[Selection];
}
