// // copyright mkurtt96
//
// #pragma once
//
// #include "CoreMinimal.h"
// #include "AbilitySystemComponent.h"
// #include "AttributeSet.h"
// #include "NativeGameplayTags.h"
// #include "PWAttributeSet_Tagged.h"
// #include "PWAttributeSet_BasicAttribute.generated.h"
//
// //Empty Basic Attribute macro. 
// #define DeclareBasicAttributeSet(Name)\
// DeclareGameplayTag_Basic(Name);\
// UCLASS()\
// class PWATTRIBUTES_API UAS_##Name## : public UPWAttributeSet_BasicAttribute \
// { GENERATED_BODY() };
// #define DefineBasicAttributeSet(Name)\
// DefineGameplayTags_Basic(Name);
//
// #define DeclareBasicAttribute(Name)\
// UPROPERTY(VisibleAnywhere, BlueprintReadOnly) \
// TObjectPtr<class UPWAttributeSet_BasicAttribute> Name;
// #define DefineBasicAttribute(Name)\
// Name = CreateDefaultSubobject<UAS_##Name##>(#Name);
//
// //put in .h outside class
// #define DeclareAttributeGameplayTag_Basic(Name) \
// namespace PWTags::Attributes { \
// UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_##Name##_Value);}
// //put in .cpp outside functions.
// #define DefineAttributeGameplayTags_Basic(Name) \
// namespace PWTags::Attributes { \
// UE_DEFINE_GAMEPLAY_TAG(Attribute_##Name##_Value,"Attribute."#Name".Value");}
//
// UE_DECLARE_GAMEPLAY_TAG_EXTERN()
// UCLASS()
// class PWATTRIBUTES_API UPWAttributeSet_BasicAttribute : public UPWAttributeSet_Tagged
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
//
// 	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
// 	
// 	UFUNCTION() void OnRep_Value(const FGameplayAttributeData& OldValue) const;
// };
