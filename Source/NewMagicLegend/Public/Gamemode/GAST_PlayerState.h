
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"

#include "GAST_PlayerState.generated.h"


class ULevelUpInfo;
class UAttributeSet;
class UAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChangedSignature,int32);

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
	FORCEINLINE int32 GetCurrentXP()const {return XP;}
	FORCEINLINE int32 GetAttributePoints() const {return AttributePoints;}
	FORCEINLINE int32 GetSpellPoints() const {return SpellPoints;}

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent>AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet>AttributeSet;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="LevelInformation")
	TObjectPtr<ULevelUpInfo>LevelUpInformation;

	/*----------------- 处理经验值和升级 -----------------------------------*/
	FOnPlayerStateChangedSignature OnXPChangeDelegate;
	FOnPlayerStateChangedSignature OnLevelChangeDelegate;
	FOnPlayerStateChangedSignature OnSpellPointsChangeDelegate;
	FOnPlayerStateChangedSignature OnAttributePointsChangeDelegate;
	
	void SetXP(int32 InXP);
	void AddToXP(int32 InXP);

	void SetLevel(int32 InLevel);
	void AddToLevel(int32 InLevel);

	void AddToSpelPoints(int32 InSpellPoints);
	void AddToAttributePoints(int32 InAttributePoints);
	
private:
	UPROPERTY(VisibleAnywhere,ReplicatedUsing=OnRep_Level,Category="PlayerClassDefaults")
	int32 Level=1;

	UPROPERTY(VisibleAnywhere,ReplicatedUsing=OnRep_XP,Category="PlayerClassDefaults")
	int32 XP=0;

	UPROPERTY(VisibleAnywhere,ReplicatedUsing=OnRep_SpellPoints,Category="PlayerClassDefaults")
	int32 SpellPoints=0;

	UPROPERTY(VisibleAnywhere,ReplicatedUsing=OnRep_AttributePoints,Category="PlayerClassDefaults")
	int32 AttributePoints=0;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UFUNCTION()
	void OnRep_XP(int32 OldXP);

	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints);

	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);
	
};


