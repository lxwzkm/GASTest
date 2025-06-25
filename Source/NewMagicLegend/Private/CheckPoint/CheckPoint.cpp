// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckPoint/CheckPoint.h"

#include "Components/SphereComponent.h"
#include "Interaction/PlayerInterface.h"

ACheckPoint::ACheckPoint(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	CheckPointMesh=CreateDefaultSubobject<UStaticMeshComponent>("CheckPointMesh");
	CheckPointMesh->SetupAttachment(RootComponent);
	CheckPointMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	CheckPointMesh->SetCollisionResponseToAllChannels(ECR_Block);

	SphereComponent=CreateDefaultSubobject<USphereComponent>("Sphere");
	SphereComponent->SetupAttachment(CheckPointMesh);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
}

void ACheckPoint::HandleGlowEffects()
{
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
	UMaterialInstanceDynamic* DynamicMaterial=UMaterialInstanceDynamic::Create(CheckPointMesh->GetMaterial(0),this);
	CheckPointMesh->SetMaterial(0,DynamicMaterial);
	CheckPointReached(DynamicMaterial);
}

void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this,&ACheckPoint::SphereOverlap);
}

void ACheckPoint::SphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_SaveProgess(OtherActor,PlayerStartTag);
		HandleGlowEffects();
	}
}
