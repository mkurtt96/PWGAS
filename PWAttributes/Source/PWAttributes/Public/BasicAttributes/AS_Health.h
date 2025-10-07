﻿// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "AttributeBases/PWAttributeSet_RegeneratingAttribute.h"
#include "AS_Health.generated.h"

DeclareGameplayTag_Regenerating(Health);

UCLASS()
class PWATTRIBUTES_API UAS_Health : public UPWAttributeSet_RegeneratingAttribute
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category="Meta")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(ThisClass, IncomingDamage);

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
};
