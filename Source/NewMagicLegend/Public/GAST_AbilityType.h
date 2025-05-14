#pragma once
#include "GameplayEffectTypes.h"
#include "GAST_AbilityType.generated.h"

/*P149
 * uint8 A=1=0000 0001;
 * uint8 B=0=0000 0010;
 *       A|B=0000 0011;
 *
 * uint8 A=1=0000 0001;
 *      1<<2=0000 0100;
 *      1<<0=0000 0001;
 * if(RepBits & 1<<0) 相当于在检查最后一个位是否为1;
 * if(RepBits & 1<<2) 想当于在检查倒数第三个位是否为1;
 */

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()
	FDamageEffectParams(){}

	UPROPERTY()
	TObjectPtr<UObject>WorldContext=nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent>SourceASC=nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent>TargetASC=nullptr;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageEffectClass=nullptr;

	UPROPERTY()
	float BaseDamage=0;

	UPROPERTY()
	int32 AbilityLevel=1;

	UPROPERTY()
	FGameplayTag DamageType=FGameplayTag();

	UPROPERTY()
	float Debuff_Chance=0.f;

	UPROPERTY()
	float Debuff_Damage=0.f;

	UPROPERTY()
	float Debuff_Duration=0.f;

	UPROPERTY()
	float Debuff_Frequency=0.f;
	
};


USTRUCT(BlueprintType)
struct FMyGameplayEffectContext:public FGameplayEffectContext
{
	GENERATED_BODY()
public:

	bool IsCriticalHit()const{return bCriticalHit;}
	bool IsBlockHit()const{return bBlockHit;}
	bool IsSuccessDebuff()const{return bIsSuccessfulDebuff;}
	float GetDebuffDamage()const{return Debuff_Damage;}
	float GetDebuffDuration()const{return Debuff_Duration;}
	float GetDebuffFrequency()const{return Debuff_Frequency;}
	TSharedPtr<FGameplayTag> GetDamageType()const{return DamageType;}
	
	void SetIsCriticalHit(const bool& InIsCriticalHit){bCriticalHit=InIsCriticalHit;}
	void SetIsBlockHit(const bool& InIsBlockHit){bBlockHit=InIsBlockHit;}
	void SetIsSuccessDebuff(const bool& InIsSuccess){bIsSuccessfulDebuff=InIsSuccess;}
	void SetDebuff_Damage(const float& InDebuffDamage){Debuff_Damage=InDebuffDamage;}
	void SetDebuff_Duration(const float& InDebuffDuration){Debuff_Duration=InDebuffDuration;}
	void SetDebuff_Frequency(const float& InFrequency){Debuff_Frequency=InFrequency;}
	void SetDamageType(const TSharedPtr<FGameplayTag>& InDamageType){DamageType=InDamageType;}
	
	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		//TODO:修改成自定义的Context()类型
		return FMyGameplayEffectContext::StaticStruct();
	}

	/** Custom serialization, subclasses must override this */
	/**
	 * @param FArchive& -数据读写容器  核心数据流接口，负责二进制数据的读写操作
	 * @param UPackageMap -对象引用解析器 处理UObject引用的网络序列化
	 * @param bool -操作结果指示器  报告序列化/反序列化是否成功
	 */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FMyGameplayEffectContext* Duplicate() const
	{
		FMyGameplayEffectContext* NewContext = new FMyGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
	
protected:

	UPROPERTY()
	bool bCriticalHit=false;

	UPROPERTY()
	bool bBlockHit=false;

	UPROPERTY()
	bool bIsSuccessfulDebuff=false;

	UPROPERTY()
	float Debuff_Damage=0.f;

	UPROPERTY()
	float Debuff_Duration=0.f;

	UPROPERTY()
	float Debuff_Frequency=0.f;
	
	TSharedPtr<FGameplayTag> DamageType;
};

//此模板特化是 FGameplayEffectContext 能正确参与 UE 网络同步和内存管理的基石，缺少这些配置会导致运行时错误或数据不一致。
template<>
struct TStructOpsTypeTraits< FMyGameplayEffectContext > : public TStructOpsTypeTraitsBase2< FMyGameplayEffectContext >
{
	enum
	{
		WithNetSerializer = true,//启用该结构体的自定义网络序列化。
		WithCopy = true		    // 允许结构体的深拷贝（Deep Copy）。
	};
};
