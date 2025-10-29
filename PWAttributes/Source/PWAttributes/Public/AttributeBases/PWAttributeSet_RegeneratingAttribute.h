// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "NativeGameplayTags.h"
#include "PWAttributeSet_Tagged.h"
#include "PWAttributeSet_RegeneratingAttribute.generated.h"

#define RegeneratingAttribute(Name)\
UPROPERTY(VisibleAnywhere, BlueprintReadOnly) \
TObjectPtr<class UPWAttributeSet_RegeneratingAttribute> Name;

//put in .h outside class
#define DeclareGameplayTag_Regenerating(Name) \
namespace PWTags::Attributes { \
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_##Name##_Value); \
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_##Name##_Max); \
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_##Name##_Regen); \
}

//put in .cpp outside functions.
#define DefineGameplayTags_Regenerating(Name) \
namespace PWTags::Attributes { \
UE_DEFINE_GAMEPLAY_TAG(Attribute_##Name##_Value,"Attribute."#Name".Value"); \
UE_DEFINE_GAMEPLAY_TAG(Attribute_##Name##_Max, "Attribute."#Name".Max"); \
UE_DEFINE_GAMEPLAY_TAG(Attribute_##Name##_Regen, "Attribute."#Name".Regen"); \
}

/**
 * 
 */
UCLASS()
class PWATTRIBUTES_API UPWAttributeSet_RegeneratingAttribute : public UPWAttributeSet_Tagged
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category="Resource", ReplicatedUsing=OnRep_Value)
	FGameplayAttributeData Value;
	ATTRIBUTE_ACCESSORS(ThisClass, Value);
	UPROPERTY(BlueprintReadOnly, Category="Resource", ReplicatedUsing=OnRep_Max)
	FGameplayAttributeData Max;
	ATTRIBUTE_ACCESSORS(ThisClass, Max);
	UPROPERTY(BlueprintReadOnly, Category="Resource", ReplicatedUsing=OnRep_Regen)
	FGameplayAttributeData Regen;
	ATTRIBUTE_ACCESSORS(ThisClass, Regen);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_Value(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_Max(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_Regen(const FGameplayAttributeData& OldValue) const;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
};
