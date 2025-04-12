// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MyBTService_FindNearestPlayer.h"

#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UMyBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* OwnerPawn=AIOwner->GetPawn();

	const FName TargetTag=OwnerPawn->ActorHasTag(FName("Player"))?FName("Enemy"):FName("Player");
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(OwnerPawn,TargetTag,FoundActors);

	float NearestDistance=TNumericLimits<float>::Max();
	AActor* NearestActor=nullptr;
	for (AActor* FoundActor : FoundActors)
	{
		float Distance= OwnerPawn->GetDistanceTo(FoundActor);
		if (Distance<NearestDistance)
		{
			NearestDistance=Distance;
			NearestActor=FoundActor;
		}
	}
	UBTFunctionLibrary::SetBlackboardValueAsObject(this,TargetToFollowSelector,NearestActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this,DistanceToFollowSelector,NearestDistance);
}
