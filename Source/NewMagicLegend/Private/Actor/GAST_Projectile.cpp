// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/GAST_Projectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GAST_AbilitySystemLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "NewMagicLegend/NewMagicLegend.h"

// Sets default values
AGAST_Projectile::AGAST_Projectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates=true;//在服务器生成，客户端只有复制品

	SphereComponent=CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(SphereComponent);
	SphereComponent->SetCollisionObjectType(ECC_Projectile);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	

	ProjectileMovementComponent=CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovementComponent->InitialSpeed=550.f;
	ProjectileMovementComponent->MaxSpeed=550.f;
	ProjectileMovementComponent->ProjectileGravityScale=0.f;

	
}

void AGAST_Projectile::Destroyed()
{
	if (!bHit&&!HasAuthority())
	{
		UGameplayStatics::PlaySoundAtLocation(this,ImpactSound,GetActorLocation(),FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ImpactEffect,GetActorLocation());
		if (LoopSoundComponent) LoopSoundComponent->Stop();
		bHit=true;
	}
	Super::Destroyed();
}

void AGAST_Projectile::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this,&AGAST_Projectile::SphereOverlap);
	SetLifeSpan(LifeSpan);

	LoopSoundComponent= UGameplayStatics::SpawnSoundAttached(LoopSound,GetRootComponent());
	
}

void AGAST_Projectile::OnHit()
{
	UGameplayStatics::PlaySoundAtLocation(this,ImpactSound,GetActorLocation(),FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ImpactEffect,GetActorLocation());
	if (LoopSoundComponent) LoopSoundComponent->Stop();
	bHit=true;
}

void AGAST_Projectile::SphereOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* SourceActor=DamageEffectParams.SourceASC->GetAvatarActor();
	if (SourceActor == OtherActor)return;

	if (!bHit)OnHit();

	if (!UGAST_AbilitySystemLibrary::IsNotFriend(OtherActor,SourceActor))
		return;
	
	if (HasAuthority())
	{

		if (UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			DamageEffectParams.TargetASC=TargetASC;
			UGAST_AbilitySystemLibrary::ApplyDamageEffectToTarget(DamageEffectParams);
		}
		
		Destroy();
	}
	else
	{
		bHit=true;
	}
}
	
