// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncLoadImage.generated.h"

struct FStreamableHandle;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnImageLoadComplete, const TArray<UTexture2D*>&, LoadedTextures, bool,bSuccess);

struct FRawImageData
{
	int32 Width;
	int32 Height;
	TArray<uint8> RawData;
};
/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API UAsyncLoadImage : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "Async Load Image")
	static UAsyncLoadImage* AsyncLoadImageToFile(const UObject* WorldContextObject,const FString& Path,const FString& FileExtension);

	UPROPERTY(BlueprintAssignable)
	FOnImageLoadComplete OnImageLoadComplete;

protected:

	FString TargetFolderPath;
	FString TargetFileExtension;
	const UObject* WorldContextObject;
	TArray<FSoftObjectPath> TexturesToLoad;
	TSharedPtr<FStreamableHandle> StreamableHandle;
	virtual void Activate() override;

private:
	void LoadImage();
	UTexture2D* LoadTextureFromFile(const FString& Path);
	void CreateTexturesOnGameThread(TArray<FRawImageData>&& RawImages);
};
