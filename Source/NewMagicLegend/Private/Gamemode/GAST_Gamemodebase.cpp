// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemode/GAST_Gamemodebase.h"

#include "EngineUtils.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"
#include "Gamemode/GAST_GameInstance.h"
#include "Gamemode/LoadSlotSaveGame.h"
#include "Interaction/SavedInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "UI/MVVM/MVVM_LoadSlotViewModel.h"

void AGAST_Gamemodebase::SaveLoadSlot(UMVVM_LoadSlotViewModel* LoadSlot, int32 LoadSlotIndex)
{
	DeleteSlot(LoadSlot->GetLoadSlotName(), LoadSlotIndex);
	
	USaveGame* SaveGameObj= UGameplayStatics::CreateSaveGameObject(SlotSaveGameClass);
	ULoadSlotSaveGame* LoadSlotSaveGame=Cast<ULoadSlotSaveGame>(SaveGameObj);
	LoadSlotSaveGame->PlayerName=LoadSlot->GetPlayerName();
	LoadSlotSaveGame->SlotStatus=LoadSlot->SaveSlotStatus;
	LoadSlotSaveGame->MapName=LoadSlot->GetMapName();
	LoadSlotSaveGame->MapAssetName=LoadSlot->MapAssetName;
	LoadSlotSaveGame->PlayerStartTag=LoadSlot->PlayerStartTag;

	UGameplayStatics::SaveGameToSlot(LoadSlotSaveGame,LoadSlot->GetLoadSlotName(),LoadSlotIndex);
}

ULoadSlotSaveGame* AGAST_Gamemodebase::GetSaveDataFromSlot(const FString& LoadSlotName, int32 Index)
{
	USaveGame* SaveGameObject=nullptr;
	if (UGameplayStatics::DoesSaveGameExist(LoadSlotName,Index))
	{
		SaveGameObject=UGameplayStatics::LoadGameFromSlot(LoadSlotName,Index);
	}
	else
	{
		SaveGameObject=UGameplayStatics::CreateSaveGameObject(SlotSaveGameClass);
	}
	ULoadSlotSaveGame* LoadSlotViewModel=Cast<ULoadSlotSaveGame>(SaveGameObject);
	return LoadSlotViewModel;
}

void AGAST_Gamemodebase::DeleteSlot(const FString& LoadSlotName, int32 Index)
{
	const bool bLoadSlotExist=UGameplayStatics::DoesSaveGameExist(LoadSlotName,Index);
	if(bLoadSlotExist)
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlotName,Index);
	}
}

void AGAST_Gamemodebase::TravelToMap(UMVVM_LoadSlotViewModel* LoadSlot)
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(LoadSlot,Maps.FindChecked(LoadSlot->GetMapName()));
}

ULoadSlotSaveGame* AGAST_Gamemodebase::RetrievelSaveGameData()
{
	UGAST_GameInstance* MyGameInstance=Cast<UGAST_GameInstance>(GetGameInstance());
	const FString InLoadSlotName=MyGameInstance->LoadSlotName;
	const int32 InLoadSlotIndex=MyGameInstance->LoadSlotIndex;

	return GetSaveDataFromSlot(InLoadSlotName,InLoadSlotIndex);
}

void AGAST_Gamemodebase::SaveInGameProgessData(ULoadSlotSaveGame* SaveData)
{
	UGAST_GameInstance* MyGameInstance=Cast<UGAST_GameInstance>(GetGameInstance());
	const FString InLoadSlotName=MyGameInstance->LoadSlotName;
	const int32 InLoadSlotIndex=MyGameInstance->LoadSlotIndex;

	MyGameInstance->PlayerStartTag=SaveData->PlayerStartTag;

	UGameplayStatics::SaveGameToSlot(SaveData,InLoadSlotName,InLoadSlotIndex);
}

void AGAST_Gamemodebase::SavedWorldState(UWorld* World,const FString& DestinationMapAssetName)
{
	FString CurrentMapName=World->GetMapName();
	CurrentMapName.RemoveFromStart(World->StreamingLevelsPrefix);
	UGAST_GameInstance* MyGameInstance=Cast<UGAST_GameInstance>(GetGameInstance());
	check(MyGameInstance);
	if (ULoadSlotSaveGame* SaveGame=GetSaveDataFromSlot(MyGameInstance->LoadSlotName,MyGameInstance->LoadSlotIndex))
	{
		if (!DestinationMapAssetName.IsEmpty())
		{
			SaveGame->MapAssetName=DestinationMapAssetName;
			SaveGame->MapName=GetMapNameFromMapAssetName(DestinationMapAssetName);
		}
		
		if (!SaveGame->HasMap(CurrentMapName))
		{
			FSavedMap NewSavedMap;
			NewSavedMap.MapName=CurrentMapName;
			SaveGame->SavedMaps.Add(NewSavedMap);
		}

		FSavedMap CurrentSavedMap=SaveGame->GetSavedMapByMapName(CurrentMapName);
		CurrentSavedMap.SavedActors.Empty();

		for (FActorIterator It(World); It; ++It)//遍历世界里的Actors
		{
			AActor* Actor=*It;

			if (!IsValid(Actor) || !Actor->Implements<USavedInterface>())continue;

			FSavedActor NewActor;
			NewActor.ActorName=Actor->GetFName();
			NewActor.ActorTransform=Actor->GetTransform();

			FMemoryWriter Writer(NewActor.Bytes);//将数据写入内存（TArray<uint8>）
			FObjectAndNameAsStringProxyArchive Archive(Writer,true);//特化的Archive，能序列化UObject引用为字符串形式，适用于SaveGame场景
			Archive.ArIsSaveGame=true;//关键标记！如果你在UProperty上加了SaveGame标记（UPROPERTY(SaveGame)），则只有在这个标记为 true 时这些属性才会被序列化！

			Actor->Serialize(Archive);//将Actor序列化,只序列化有SaveGame标记的属性变量
			CurrentSavedMap.SavedActors.AddUnique(NewActor);
		}

		for (FSavedMap& Map:SaveGame->SavedMaps)
		{
			if (Map.MapName==CurrentMapName)
			{
				Map=CurrentSavedMap;//替换原有的存档数据
			}
		}
		UGameplayStatics::SaveGameToSlot(SaveGame,MyGameInstance->LoadSlotName,MyGameInstance->LoadSlotIndex);
	}
}

void AGAST_Gamemodebase::LoadWorldState(UWorld* World)
{
	FString CurrentMapName=World->GetMapName();
	CurrentMapName.RemoveFromStart(World->StreamingLevelsPrefix);

	UGAST_GameInstance* MyGameInstance=Cast<UGAST_GameInstance>(GetGameInstance());
	check(MyGameInstance);

	if (UGameplayStatics::DoesSaveGameExist(MyGameInstance->LoadSlotName,MyGameInstance->LoadSlotIndex))
	{
		ULoadSlotSaveGame* SaveGame=Cast<ULoadSlotSaveGame>(UGameplayStatics::LoadGameFromSlot(MyGameInstance->LoadSlotName,MyGameInstance->LoadSlotIndex));
		auto SavedMap=SaveGame->GetSavedMapByMapName(CurrentMapName);
		for (FActorIterator it(World);it;++it)
		{
			AActor* Actor=*it;
			if (!Actor->Implements<USavedInterface>())continue;

			for (FSavedActor& SavedActor:SavedMap.SavedActors)
			{
				if (SavedActor.ActorName==Actor->GetFName())
				{
					if (ISavedInterface::Execute_ShouldReplaceTransform(Actor))
					{
						Actor->SetActorTransform(SavedActor.ActorTransform);
					}

					FMemoryReader MemoryReader(SavedActor.Bytes);
					FObjectAndNameAsStringProxyArchive Archive(MemoryReader,true);
					Archive.ArIsSaveGame=true;
					Actor->Serialize(Archive);

					ISavedInterface::Execute_LoadActor(Actor);
				}
			}
		}
	}
}

FString AGAST_Gamemodebase::GetMapNameFromMapAssetName(const FString& MapAssetName) const
{
	for (auto& Map:Maps)
	{
		if (Map.Value.ToSoftObjectPath().GetAssetName()==MapAssetName)
		{
			return Map.Key;
		}
	}
	return  FString();
}

AActor* AGAST_Gamemodebase::ChoosePlayerStart_Implementation(AController* Player)
{
	UGAST_GameInstance* MyGameInstance=Cast<UGAST_GameInstance>(GetGameInstance());
	TArray<AActor*> AllPlayerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),APlayerStart::StaticClass(),AllPlayerActors);
	if (AllPlayerActors.Num() > 0)
	{
		AActor* SelectedPlayerActor=AllPlayerActors[0];
		for (AActor* PlayerActor : AllPlayerActors)
		{
			if (APlayerStart* PlayerStart=Cast<APlayerStart>(PlayerActor))
			{
				if (PlayerStart->PlayerStartTag==MyGameInstance->PlayerStartTag)
				{
					SelectedPlayerActor=PlayerActor;
					break;
				}
			}
		}
		return SelectedPlayerActor;
	}
	return nullptr;
}

void AGAST_Gamemodebase::CharacterDead(ACharacter* DeadCharacter)
{
	ULoadSlotSaveGame* SaveGame= RetrievelSaveGameData();
	if (!IsValid(SaveGame))return;

	UGameplayStatics::OpenLevel(DeadCharacter,FName(SaveGame->MapAssetName));
}

void AGAST_Gamemodebase::BeginPlay()
{
	Super::BeginPlay();

	Maps.Add(DefaultMapName,DefaultMap);
}
