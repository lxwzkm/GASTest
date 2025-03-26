// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "GAST_AssetManager.generated.h"

/**
 * 单例模式的资产信息管理
 */
UCLASS()
class NEWMAGICLEGEND_API UGAST_AssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	static UGAST_AssetManager& Get();
protected:
	//重写父类虚函数，作用是开始时进行加载,初始化GameplayTags
	virtual void StartInitialLoading() override;
};
