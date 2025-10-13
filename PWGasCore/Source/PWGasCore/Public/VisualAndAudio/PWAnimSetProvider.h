// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PWAnimSetProvider.generated.h"

struct FGameplayTag;

UINTERFACE()
class UPWAnimSetProvider : public UInterface
{
	GENERATED_BODY()
};

class PWGASCORE_API IPWAnimSetProvider
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Animation")
	UAnimMontage* GetMontageForTag(FGameplayTag AnimTag) const;
};
