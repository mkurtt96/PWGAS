// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "PWAttributeSet_Tagged.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

struct FGameplayTag;
/**
 * 
 */
UCLASS()
class PWATTRIBUTES_API UPWAttributeSet_Tagged : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category="PW|Attributes|Tags")
	TMap<FGameplayTag, FName> TagToPropertyName;

	UFUNCTION(BlueprintCallable, Category="PW|Attributes|Tags")
	bool GetAttributeForTag(const FGameplayTag& Tag, FGameplayAttribute& OutAttribute) const;

	UFUNCTION(BlueprintPure, Category="PW|Attributes|Tags")
	static FGameplayAttribute MakeAttribute(const UClass* SetClass, FName PropertyName);
};
