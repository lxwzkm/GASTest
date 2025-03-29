// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GAST_InputConfig.h"
#include "GAST_EnhancedInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API UGAST_EnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
public:

	/**
	 * @brief 这是一个将Config中的行为、Tag与不同按键的回调函数进行绑定的函数，它会遍历输入配置中的每个输入动作，依据不同的触发事件（开始、触发、完成）将输入动作与相应的函数进行绑定。并将Tag传送给按键的回调函数
	 * @InputConfig 是GAST_InputConfig文件
	 * @Object Object类
	 * @PressedFunc、@HeldFunc、@ReleasedFunc 不同按键状态的回调函数
	 */
	template<class UserClass,typename PressedFuncType,typename HeldFuncType,typename ReleasedFuncType>
	void BindAbilityActions(const UGAST_InputConfig* InputConfig,UserClass* Object,PressedFuncType PressedFunc,HeldFuncType HeldFunc,ReleasedFuncType ReleasedFunc);
};
/*
 * 
 */
template <class UserClass, typename PressedFuncType, typename HeldFuncType, typename ReleasedFuncType>
void UGAST_EnhancedInputComponent::BindAbilityActions(const UGAST_InputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, HeldFuncType HeldFunc, ReleasedFuncType ReleasedFunc)
{
	check(InputConfig);

	for (const auto& Action:InputConfig->AbilityInputActions)
	{
		if (Action.AbilityInputAction&&Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(Action.AbilityInputAction,ETriggerEvent::Started,Object,PressedFunc,Action.InputTag);
			}
			if (HeldFunc)
			{
				BindAction(Action.AbilityInputAction,ETriggerEvent::Triggered,Object,HeldFunc,Action.InputTag);
			}
			if (ReleasedFunc)
			{
				BindAction(Action.AbilityInputAction,ETriggerEvent::Completed,Object,ReleasedFunc,Action.InputTag);
			}
		}
	}
}
