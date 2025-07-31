// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckPoint/MapEntrance.h"


#include "Gamemode/GAST_Gamemodebase.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

class AGAST_Gamemodebase;

AMapEntrance::AMapEntrance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

void AMapEntrance::LoadActor_Implementation()
{
	//取消父类的材质发光
}

void AMapEntrance::HightlightActor_Implementation()
{
	CheckPointMesh->SetRenderCustomDepth(true);
}

void AMapEntrance::SphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		bCheckPointReached=true;
		if (AGAST_Gamemodebase* MyGm=Cast<AGAST_Gamemodebase>(UGameplayStatics::GetGameMode(this)))
		{
			MyGm->SavedWorldState(GetWorld(),DestinationMap.ToSoftObjectPath().GetAssetName());
		}
		IPlayerInterface::Execute_SaveProgess(OtherActor,DestinationPlayerStartTag);
		UGameplayStatics::OpenLevelBySoftObjectPtr(this,DestinationMap);
	}
}
