// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PWTargetingInterfaces.generated.h"

struct FPWTargetingResult;

UINTERFACE(BlueprintType)
class UPWTargetingSource : public UInterface { GENERATED_BODY() };
class IPWTargetingSource
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool Sample(class APlayerController* PC, FPWTargetingResult& Out);
};

UINTERFACE(BlueprintType)
class UPWRangePolicy : public UInterface { GENERATED_BODY() };
class IPWRangePolicy
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Apply(const class AActor* Origin, float MaxRange, FPWTargetingResult& InOut);
};