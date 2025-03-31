// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTasks/TargetDataUnderMouse.h"

#include "Gamemode/GAST_PlayerCOntroller.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* Obj=NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return Obj;
}

void UTargetDataUnderMouse::Activate()
{

	if (Ability->GetCurrentActorInfo()->IsLocallyControlled())
	{//只要是本地操作，如果是服务器，就直接发送数据，如果是客户端直接发送数据，服务器绑定回调函数
		SendMouseCursor();
	}
	else
	{//不是本地操作的时候，说明应该是服务器，绑定回调函数
		
		const FGameplayAbilitySpecHandle SpecHandle=GetAbilitySpecHandle();
		const FPredictionKey OriginalPredictionKey=GetActivationPredictionKey();
		//绑定监听
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle,OriginalPredictionKey).AddUObject(this,&UTargetDataUnderMouse::OnTargetDataReplicatedCallBacks);
		//由于可能会数据先到达，所以判断是否有成功广播数据
		const bool IsDelegate= AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle,OriginalPredictionKey);
		if (!IsDelegate)
		{
			SetWaitingOnRemotePlayerData();//没有成功广播数据会让服务器等待接收数据
		}
	}
	
	
}

void UTargetDataUnderMouse::SendMouseCursor()
{

	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	
	FHitResult Hit;
	if (APlayerController* PlayerController= Ability->GetCurrentActorInfo()->PlayerController.Get())
	{
		PlayerController->GetHitResultUnderCursor(ECC_Visibility,false,Hit);
	}

	FGameplayAbilityTargetData_SingleTargetHit* Data=new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult=Hit;

	FGameplayAbilityTargetDataHandle DataHandle;
	DataHandle.Add(Data);
	
	//将向服务器发送TargetData
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())//判断当前Ability是否还在激活状态
	{
		ValiedData.Broadcast(DataHandle);
	}
	
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallBacks(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag)
{
	//通知ASC已经收到复制过来的TargetData,ASC的缓存可以被清除了
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(),GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValiedData.Broadcast(DataHandle);
	}
}
