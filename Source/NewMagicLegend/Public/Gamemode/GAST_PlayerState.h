
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"

#include "GAST_PlayerState.generated.h"


class UAttributeSet;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API AGAST_PlayerState : public APlayerState,public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AGAST_PlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	/*-------IAbilitySystemInterface-------*/
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	/*-------IAbilitySystemInterface  end-------*/
	UAttributeSet* GetAttributeSet()const;

	FORCEINLINE int32 GetPlayerLevel()const {return Level;}

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent>AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet>AttributeSet;
private:
	UPROPERTY(VisibleAnywhere,ReplicatedUsing=Rep_Level,Category="PlayerClassDefaults")
	int32 Level=1;

	UFUNCTION()
	void Rep_Level(int32 OldLevel);
	
};
