
//使用TargetData需要在一开始的时候进行初始化，本次初始化在GAST_AssetManager.cpp
#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMouseLocationSignature,const FGameplayAbilityTargetDataHandle&,DataHandle);

/**2025.04.01--deepseek
 *  客户端向服务器发送数据没法保证AbilityTask运行的时候是Active先激活还是数据先到达服务器
*  1. 服务器端的委托绑定
*  当 Activate() 在服务器端执行时（即 IsLocallyControlled 为 false），服务器会执行以下操作：
*  1.1 绑定回调函数
*  目的：注册一个回调函数 OnTargetDataReplicatedCallBacks，当客户端通过 ServerSetReplicatedTargetData 发送目标数据时，ASC 会通过 SpecHandle 和 PredictionKey 匹配到此委托并触发回调。
*  触发条件：客户端调用 ServerSetReplicatedTargetData 发送数据。
*  1.2 立即检查已有数据
*  目的：检查 ASC 是否已经缓存了与 SpecHandle 和 PredictionKey 匹配的 ：TargetData。const bool IsDelegate = AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, OriginalPredictionKey);
*  如果数据已存在：立即触发回调 OnTargetDataReplicatedCallBacks，返回 true。
*  如果数据不存在：返回 false，进入等待状态。
*  1.3 处理等待状态
*  目的：如果数据未到达（IsDelegate == false），调用 SetWaitingOnRemotePlayerData()，通知 ASC 进入等待状态。
*  等待机制：ASC 内部会挂起 Ability 的执行，直到客户端数据到达或超时。
*  2. 客户端发送数据
*  当客户端执行 SendMouseCursor() 时：
*  2.1 生成并发送数据
*  目的：将 TargetData 从客户端发送到服务器。
*  服务器处理：服务器 ASC 接收到数据后，会缓存数据并通过 SpecHandle 和 PredictionKey 查找匹配的委托。
*  3. 数据到达后的处理
*  无论数据是在委托绑定前还是绑定后到达：
*  3.1 数据在绑定前到达
*  ASC 会缓存数据。
*  当 CallReplicatedTargetDataDelegatesIfSet 被调用时，立即触发回调。
*  3.2 数据在绑定后到达
*  ASC 通过 AbilityTargetDataSetDelegate 触发回调。
 */
UCLASS()
class NEWMAGICLEGEND_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable,Category="Ability",meta=(HidePin="OwingAbility",DefaultToSelf="OwningAbility",BlueprintInternalUseOnly="true"))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FOnMouseLocationSignature ValiedData;

private:
	virtual void Activate() override;

	void SendMouseCursor();

	void OnTargetDataReplicatedCallBacks(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
};
