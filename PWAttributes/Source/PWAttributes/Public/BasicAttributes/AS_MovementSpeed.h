// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "AttributeBases/PWAttributeSet_BasicAttribute.h"
#include "AS_MovementSpeed.generated.h"

DeclareGameplayTag_Basic(MovementSpeed);

UCLASS()
class PWATTRIBUTES_API UAS_MovementSpeed : public UPWAttributeSet_BasicAttribute
{
	GENERATED_BODY()

public:
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attr, float& NewValue) override;
};
