
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
	/*-------IAbilitySystemInterface-------*/
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	/*-------IAbilitySystemInterface  end-------*/
	UAttributeSet* GetAttributeSet()const;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent>AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet>AttributeSet;
};
