// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "PWAttributeSetBase.generated.h"

struct FEffectProperties;
struct FEffectObject;
struct FGameplayTag;

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

#define DeclareReplicatedAttribute(PropertyName, CategoryName) \
UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_##PropertyName, Category = CategoryName) \
FGameplayAttributeData PropertyName; \
UFUNCTION() void OnRep_##PropertyName(const FGameplayAttributeData& OldValue) const \
{ GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, PropertyName, OldValue); } \
ATTRIBUTE_ACCESSORS(ThisClass, PropertyName)

#define DeclareLocalAttribute(PropertyName, CategoryName) \
UPROPERTY(BlueprintReadOnly, Category = CategoryName) \
FGameplayAttributeData PropertyName; \
ATTRIBUTE_ACCESSORS(ThisClass, PropertyName)

UCLASS()
class PWATTRIBUTES_API UPWAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPWAttributeSetBase();

	UPROPERTY()
	TMap<FGameplayTag, FGameplayAttribute> TagsToAttributes;

	UFUNCTION(BlueprintCallable, Category="PW|Attributes")
	virtual float GetAttributeValue(const FGameplayTag& AttributeTag) const;
	UFUNCTION(BlueprintCallable, Category="PW|Attributes")
	virtual void SetAttributeValue(const FGameplayTag& AttributeTag, float NewValue);
	UFUNCTION(BlueprintCallable, Category="PW|Attributes")
	virtual FGameplayAttribute GetAttribute(const FGameplayTag& AttributeTag) const;

	virtual void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props);
	virtual void SetEffectObject(FEffectObject& Props, const FGameplayAbilityActorInfo* ActorInfo);
	
};
