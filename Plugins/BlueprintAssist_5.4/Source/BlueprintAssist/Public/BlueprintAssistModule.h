// Copyright fpwong. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "UObject/ObjectMacros.h"
#include "UObject/WeakObjectPtr.h"

class UBARootObject;
class FBlueprintAssistGraphPanelNodeFactory;

class FBlueprintAssistModule final : public IModuleInterface
{
public:
	virtual void StartupModule() override;

	virtual void ShutdownModule() override;

	void BindLiveCodingSound();
	void UnbindLiveCodingSound();

	void RegisterSettings();

	static FBlueprintAssistModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FBlueprintAssistModule>("BlueprintAssist");
	}

	static bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("BlueprintAssist");
	}

	UBARootObject* GetRootObject() const
	{
		return RootObject.Get();
	};

private:
	bool bWasModuleInitialized = false;

	TSharedPtr<FBlueprintAssistGraphPanelNodeFactory> BANodeFactory;

	TWeakObjectPtr<UBARootObject> RootObject;

	FDelegateHandle LiveCodingDelegate;

	void OnPostEngineInit();
};
