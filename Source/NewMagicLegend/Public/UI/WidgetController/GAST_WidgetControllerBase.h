//负责从model部分获取数据，发给view，从view部分收到命令，转发给model
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "UObject/NoExportTypes.h"
#include "GAST_WidgetControllerBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

USTRUCT(BlueprintType)//这个结构体是为了方便设置WidgetController内的四个参数所设立的
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams(){}
	FWidgetControllerParams(APlayerController*PC,APlayerState*PS,UAttributeSet*AS,UAbilitySystemComponent*ASC)
	:PlayerController(PC)
	,PlayerState(PS)
	,AttributeSet(AS)
	,AbilitySystemComponent(ASC){}

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<APlayerController>PlayerController;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<APlayerState>PlayerState;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UAttributeSet>AttributeSet;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent>AbilitySystemComponent;
};

/**
 * 
 */
UCLASS()
class NEWMAGICLEGEND_API UGAST_WidgetControllerBase : public UObject
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)//这个函数是为了填充WidgetController内的变量
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

	//提供给子类重写的虚函数，作用是广播属性的初始化数值
	virtual void BroadcastInitValues();
	//提供给子类重写的虚函数，作用是用来绑定属性变化时的回调函数
	virtual void BindCallbacksToDependencies();
	
	//获取数据的四个model部分
	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<APlayerState>PS;

	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<UAbilitySystemComponent>ASC;

	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<UAttributeSet>AS;

	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<APlayerController>PC;
};
