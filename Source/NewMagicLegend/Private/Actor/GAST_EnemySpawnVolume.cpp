// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/GAST_EnemySpawnVolume.h"

#include "Actor/GAST_EnemySpawnPoint.h"
#include "Components/BoxComponent.h"
#include "Interaction/PlayerInterface.h"


AGAST_EnemySpawnVolume::AGAST_EnemySpawnVolume()
{

	PrimaryActorTick.bCanEverTick = false;

	BoxComponent=CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxComponent->SetupAttachment(GetRootComponent());
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	BoxComponent->SetCollisionObjectType(ECC_WorldStatic);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
}

void AGAST_EnemySpawnVolume::LoadActor_Implementation()
{
	if (bReached)
	{
		BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AGAST_EnemySpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this,&AGAST_EnemySpawnVolume::BoxOverlap);
}

void AGAST_EnemySpawnVolume::BoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->Implements<UPlayerInterface>()||bReached)return;

	bReached=true;
	for (auto point:TargetPoints)
	{
		point->SpawnEnemy();
	}
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

