// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PointCollection.h"

#include "GAST_AbilitySystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


APointCollection::APointCollection()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	PT_0=CreateDefaultSubobject<USceneComponent>("PT_0");
	SetRootComponent(PT_0);
	ImmutablePoints.Add(PT_0);

	PT_1=CreateDefaultSubobject<USceneComponent>("PT_1");
	PT_1->SetupAttachment(GetRootComponent());
	ImmutablePoints.Add(PT_1);
	PT_2=CreateDefaultSubobject<USceneComponent>("PT_2");
	PT_2->SetupAttachment(GetRootComponent());
	ImmutablePoints.Add(PT_2);
	PT_3=CreateDefaultSubobject<USceneComponent>("PT_3");
	PT_3->SetupAttachment(GetRootComponent());
	ImmutablePoints.Add(PT_3);
	PT_4=CreateDefaultSubobject<USceneComponent>("PT_4");
	PT_4->SetupAttachment(GetRootComponent());
	ImmutablePoints.Add(PT_4);
	PT_5=CreateDefaultSubobject<USceneComponent>("PT_5");
	PT_5->SetupAttachment(GetRootComponent());
	ImmutablePoints.Add(PT_5);
	PT_6=CreateDefaultSubobject<USceneComponent>("PT_6");
	PT_6->SetupAttachment(GetRootComponent());
	ImmutablePoints.Add(PT_6);
	PT_7=CreateDefaultSubobject<USceneComponent>("PT_7");
	PT_7->SetupAttachment(GetRootComponent());
	ImmutablePoints.Add(PT_7);
	PT_8=CreateDefaultSubobject<USceneComponent>("PT_8");
	PT_8->SetupAttachment(GetRootComponent());
	ImmutablePoints.Add(PT_8);
	PT_9=CreateDefaultSubobject<USceneComponent>("PT_9");
	PT_9->SetupAttachment(GetRootComponent());
	ImmutablePoints.Add(PT_9);
	PT_10=CreateDefaultSubobject<USceneComponent>("PT_10");
	PT_10->SetupAttachment(GetRootComponent());
	ImmutablePoints.Add(PT_10);
	
}

TArray<USceneComponent*> APointCollection::GetGroundPoints(FVector GroundLocaiton, int32 NumPoints, float YawOverride)
{
	checkf(ImmutablePoints.Num()>=NumPoints,TEXT("获取点的数量越界"));

	TArray<USceneComponent*> CopyArray;
	for (auto PT:ImmutablePoints)
	{
		if (CopyArray.Num()>=NumPoints)return CopyArray;

		if (PT!=PT_0)
		{
			FVector ToPoint=PT->GetComponentLocation()-PT_0->GetComponentLocation();
			ToPoint=ToPoint.RotateAngleAxis(YawOverride,FVector::UpVector);
			PT->SetWorldLocation(PT_0->GetComponentLocation()+ToPoint);
		}
		TArray<AActor*>IgnorActors;
		UGAST_AbilitySystemLibrary::GetLivePlayersWithInRadius(this,IgnorActors,TArray<AActor*>(),1500.f,GetActorLocation());

		FVector BeginLocation=FVector(PT->GetComponentLocation().X,PT->GetComponentLocation().Y,PT->GetComponentLocation().Z+500);
		FVector EndLocation=FVector(PT->GetComponentLocation().X,PT->GetComponentLocation().Y,PT->GetComponentLocation().Z-500);

		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActors(IgnorActors);
		GetWorld()->LineTraceSingleByChannel(HitResult,BeginLocation,EndLocation,ECC_Visibility,Params);
		if (HitResult.bBlockingHit)
		{
			PT->SetWorldLocation(FVector(PT->GetComponentLocation().X,PT->GetComponentLocation().Y,HitResult.ImpactPoint.Z));
			PT->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));
		}
		CopyArray.Add(PT);
	}
	return CopyArray;
}


void APointCollection::BeginPlay()
{
	Super::BeginPlay();
	
}

