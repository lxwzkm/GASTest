// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "LoadSlotSaveGame.generated.h"

class UGameplayAbility;

UENUM()
enum SaveSlotStatus
{
	Vacant,
	EnteryName,
	Taken
};

USTRUCT()
struct FSavedActor
{
	GENERATED_BODY()

	UPROPERTY()
	FName ActorName=FName();

	UPROPERTY()
	FTransform ActorTransform=FTransform();

	//序列化保存的Actor的其他信息，只有携带SaveGame的specifier的才可以被marked
	UPROPERTY()
	TArray<uint8> Bytes;
};
inline bool operator==(const FSavedActor& lhs, const FSavedActor& rhs)
{
	return lhs.ActorName==rhs.ActorName;
}

USTRUCT()
struct FSavedMap
{
	GENERATED_BODY()

	UPROPERTY()
	FString MapName=FString();

	UPROPERTY()
	TArray<FSavedActor> SavedActors;
};

USTRUCT(BlueprintType)
struct FSavedAbility
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Default")
	TSubclassOf<UGameplayAbility> AbilityClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Default")
	FGameplayTag AbilityStatus=FGameplayTag();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Default")
	FGameplayTag AbilitySlot=FGameplayTag();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Default")
	FGameplayTag AbilityTag=FGameplayTag();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Default")
	FGameplayTag AbilityType=FGameplayTag();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Default")
	int32 AbilityLevel=1;
};

inline bool operator==(const FSavedAbility& lhs, const FSavedAbility& rhs)
{
	return lhs.AbilityTag.MatchesTagExact(rhs.AbilityTag);
}

/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API ULoadSlotSaveGame : public USaveGame
{
	GENERATED_BODY()
public:

	UPROPERTY()
	FString LoadSlotName=FString();

	UPROPERTY()
	int32 LoadSlotIndex=0;
	/*------------- 储存的信息 ------------------------*/
	UPROPERTY()
	FString PlayerName=FString("默认名称");
	UPROPERTY()
	FString MapName=FString("Map");
	UPROPERTY()
	FString MapAssetName=FString("MapAssetName");
	UPROPERTY()
	FName PlayerStartTag=FName();
	UPROPERTY()
	bool bIsFirstLoadin=true;

	//Player
	UPROPERTY()
	int32 PlayerLevel=1;
	UPROPERTY()
	int32 XP=0;
	UPROPERTY()
	int32 AttributePoints=0;
	UPROPERTY()
	int32 SpellPoints=0;

	//Attribute
	UPROPERTY()
	int32 Intelligence=0;
	UPROPERTY()
	int32 Strength=0;
	UPROPERTY()
	int32 Resilience=0;
	UPROPERTY()
	int32 Vigor=0;

	//Ability
	UPROPERTY()
	TArray<FSavedAbility> SavedAbilities;

	//Map
	UPROPERTY()
	TArray<FSavedMap> SavedMaps;

	FSavedMap GetSavedMapByMapName(const FString& InMapName);
	bool HasMap(const FString& InMapName) const;
	
	UPROPERTY()
	TEnumAsByte<SaveSlotStatus> SlotStatus=Vacant;
};
