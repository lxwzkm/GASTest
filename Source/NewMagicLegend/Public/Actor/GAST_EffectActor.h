// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GAST_EffectActor.generated.h"

class UGameplayEffect;

UCLASS()
class NEWMAGICLEGEND_API AGAST_EffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AGAST_EffectActor();

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor,TSubclassOf<UGameplayEffect> Effect);//这个函数是用来将Actor的GE用在触发重叠的人身上的
protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="ApplyEffect")
	TSubclassOf<UGameplayEffect> InstantGameplayEffect;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="ApplyEffect")
	TSubclassOf<UGameplayEffect> DurationGameplayEffect;

};
