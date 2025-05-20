// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AsyncTasks/AsyncLoadImage.h"

#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
struct FStreamableManager;

UAsyncLoadImage* UAsyncLoadImage::AsyncLoadImageToFile(const UObject* WorldContextObject,const FString& Path, const FString& FileExtension)
{
	UAsyncLoadImage* LoadImage = NewObject<UAsyncLoadImage>();
	LoadImage->WorldContextObject = WorldContextObject;
	LoadImage->TargetFolderPath = Path;
	LoadImage->TargetFileExtension = FileExtension;
	return LoadImage;
}

void UAsyncLoadImage::Activate()
{
	// IFileManager& FileManager = IFileManager::Get();
 //    
	// FString SearchPattern = TargetFolderPath / FString::Printf(TEXT("*.%s"), *TargetFileExtension);
	// TArray<FString> FilePaths;
	// FileManager.FindFiles(FilePaths, *SearchPattern, true, false);
	//
	// TArray<UTexture2D*> LoadedTextures;
	// for (const FString& FileName : FilePaths)
	// {
	// 	FString FullPath = FPaths::Combine(TargetFolderPath, FileName);
	// 	if (UTexture2D* Texture = LoadTextureFromFile(FullPath))
	// 	{
	// 		LoadedTextures.Add(Texture);
	// 	}
	// }
	//
	// OnImageLoadComplete.Broadcast(LoadedTextures, LoadedTextures.Num() > 0);
	// SetReadyToDestroy();

	

	FString SearchPattern=TargetFolderPath/FString::Printf(TEXT("*.%s"),*TargetFileExtension);
	Async(EAsyncExecution::ThreadPool,[this,SearchPattern]()
	{
		IFileManager& FileManager = IFileManager::Get();
		TArray<FString> FileNames;
		FileManager.FindFiles(FileNames, *SearchPattern, true, false);

		TArray<FRawImageData> ImageDatas;

		for (const FString& FileName : FileNames)
		{
			FString FullPath = FPaths::Combine(*TargetFolderPath, *FileName);
			TArray<uint8> FileData;

			if (!FFileHelper::LoadFileToArray(FileData, *FullPath))continue;

			EImageFormat Format = EImageFormat::PNG;
			if (TargetFileExtension == TEXT("jpg"))Format = EImageFormat::JPEG;

			IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(
				"ImageWrapper");
			TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(Format);
			if (!ImageWrapper.IsValid() || !ImageWrapper->SetCompressed(FileData.GetData(), FileData.Num()))continue;

			TArray<uint8> RawData;
			if (!ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, RawData))continue;

			FRawImageData ImageData;
			ImageData.Height = ImageWrapper->GetHeight();
			ImageData.Width = ImageWrapper->GetWidth();
			ImageData.RawData = MoveTemp(RawData);

			ImageDatas.Add(MoveTemp(ImageData));
		}
		AsyncTask(ENamedThreads::Type::GameThread, [this,ImageDatas=MoveTemp(ImageDatas)]()mutable
		{
			CreateTexturesOnGameThread(MoveTemp(ImageDatas));
		});
	});

}

UTexture2D* UAsyncLoadImage::LoadTextureFromFile(const FString& Path)
{
	TArray<uint8> FileData;
	if (!FFileHelper::LoadFileToArray(FileData, *Path)) return nullptr;

	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	EImageFormat ImageFormat = EImageFormat::JPEG;

	if (Path.EndsWith(".png")) ImageFormat = EImageFormat::PNG;
	else if (Path.EndsWith(".jpg") || Path.EndsWith(".jpeg")) ImageFormat = EImageFormat::JPEG;

	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(ImageFormat);
	if (!ImageWrapper.IsValid() || !ImageWrapper->SetCompressed(FileData.GetData(), FileData.Num())) return nullptr;

	TArray<uint8> RawData ;
	if (!ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, RawData)) return nullptr;

	UTexture2D* Texture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);
	if (!Texture) return nullptr;

	void* TextureData = Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(TextureData, RawData.GetData(), RawData.Num());
	Texture->GetPlatformData()->Mips[0].BulkData.Unlock();

	Texture->UpdateResource();
	Texture->SRGB = true;
	return Texture;
}

void UAsyncLoadImage::CreateTexturesOnGameThread(TArray<FRawImageData>&& RawImages)
{
	TArray<UTexture2D*> Textures;

	for (FRawImageData& Img : RawImages)
	{
		UTexture2D* Texture = UTexture2D::CreateTransient(Img.Width, Img.Height, PF_B8G8R8A8);
		if (!Texture) continue;

		void* TextureData = Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
		FMemory::Memcpy(TextureData, Img.RawData.GetData(), Img.RawData.Num());
		Texture->GetPlatformData()->Mips[0].BulkData.Unlock();

		Texture->SRGB = true;
		Texture->UpdateResource();

		Textures.Add(Texture);
	}

	const bool bSuccess = Textures.Num() > 0;
	OnImageLoadComplete.Broadcast(Textures, bSuccess);
	SetReadyToDestroy();
}
