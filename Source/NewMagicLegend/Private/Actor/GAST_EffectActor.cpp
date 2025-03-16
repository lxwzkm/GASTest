// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/GAST_EffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/GAST_AttributeSet.h"
#include "Components/SphereComponent.h"

AGAST_EffectActor::AGAST_EffectActor()
{
 
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh=CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SetRootComponent(StaticMesh);

	Sphere=CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());

}


void AGAST_EffectActor::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this,&AGAST_EffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this,&AGAST_EffectActor::EndOverlap);
}

void AGAST_EffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO：使用GameplayEffect来替换当前效果，目前使用Const_Cast来临时使用
	if (IAbilitySystemInterface*ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UGAST_AttributeSet* AS=Cast<UGAST_AttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UGAST_AttributeSet::StaticClass()));
		UGAST_AttributeSet* MutableAS= const_cast<UGAST_AttributeSet*>(AS);
		MutableAS->SetHealth(MutableAS->GetHealth()+25.0f);
		MutableAS->SetMana(25.f);
		Destroy();
	}
}

void AGAST_EffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}


