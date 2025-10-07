// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "NativeGameplayTags.h"
#include "PWAttributeSet_Tagged.h"
#include "PWAttributeSet_BasicAttribute.generated.h"


//put in .h outside class
#define DeclareGameplayTag_Basic(Name) \
namespace PWTags::Attributes { \
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_##Name##_Value); \
}

//put in .cpp outside functions.
#define DefineGameplayTags_Basic(Name) \
namespace PWTags::Attributes { \
UE_DEFINE_GAMEPLAY_TAG(Attribute_##Name##_Value,"Attribute."#Name".Value"); \
}

UE_DECLARE_GAMEPLAY_TAG_EXTERN()
UCLASS()
class PWATTRIBUTES_API UPWAttributeSet_BasicAttribute : public UPWAttributeSet_Tagged
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category="Resource", ReplicatedUsing=OnRep_Value)
	FGameplayAttributeData Value;
	ATTRIBUTE_ACCESSORS(ThisClass, Value);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION() void OnRep_Value(const FGameplayAttributeData& OldValue) const;
};
