// // copyright mkurtt96
//
// #pragma once
//
// #include "CoreMinimal.h"
// #include "AbilitySystemComponent.h"
// #include "AttributeSet.h"
// #include "PWAttributeSet_Tagged.h"
// #include "PWAttributeSet_ClampedAttribute.generated.h"
//
// //Empty Basic Attribute macro. 
// #define DeclareClampedAttributeSet(Name)\
// DeclareGameplayTag_Clamped(Name);\
// UCLASS()\
// class PWATTRIBUTES_API UAS_##Name## : public UPWAttributeSet_ClampedAttribute \
// { GENERATED_BODY() };
// #define DefineClampedAttributeSet(Name)\
// DefineGameplayTags_Clamped(Name);
//
// #define DeclareClampedAttribute(Name)\
// UPROPERTY(VisibleAnywhere, BlueprintReadOnly) \
// TObjectPtr<class UPWAttributeSet_ClampedAttribute> Name;
// #define DefineClampedAttribute(Name)\
// Name = CreateDefaultSubobject<UAS_##Name##>(#Name);
//
// //put in .h outside class
// #define DeclareAttributeGameplayTag_Clamped(Name) \
// namespace PWTags::Attributes { \
// UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_##Name##_Value); \
// UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_##Name##_Max);}
// //put in .cpp outside functions.
// #define DefineAttributeGameplayTags_Clamped(Name) \
// namespace PWTags::Attributes { \
// UE_DEFINE_GAMEPLAY_TAG(Attribute_##Name##_Value,"Attribute."#Name".Value"); \
// UE_DEFINE_GAMEPLAY_TAG(Attribute_##Name##_Max, "Attribute."#Name".Max");}
//
// UCLASS()
// class PWATTRIBUTES_API UPWAttributeSet_ClampedAttribute : public UPWAttributeSet_Tagged
// {
// 	GENERATED_BODY()
//
// public:
//  // =================================================
//  // =================  IMPORTANT NOTE  ==============
//  // =================================================
// 	// Simply commented out to prevent extra Attribute Tag clutter in editor if this system is not being used.
// 	// Just uncomment to use
// 	
// 	// UPROPERTY(BlueprintReadOnly, Category="Resource", ReplicatedUsing=OnRep_Value)
// 	// FGameplayAttributeData Value;
// 	// ATTRIBUTE_ACCESSORS(ThisClass, Value);
// 	// UPROPERTY(BlueprintReadOnly, Category="Resource", ReplicatedUsing=OnRep_Max)
// 	// FGameplayAttributeData Max;
// 	// ATTRIBUTE_ACCESSORS(ThisClass, Max);
//
// 	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
// 	
// 	UFUNCTION() void OnRep_Value(const FGameplayAttributeData& OldValue) const;
// 	UFUNCTION() void OnRep_Max(const FGameplayAttributeData& OldValue) const;
//
// 	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
// 	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
// };
