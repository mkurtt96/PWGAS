// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "FPWAbilityInputListener.generated.h"



UENUM(BlueprintType)
enum class EPWInputEventType : uint8
{
	Pressed,
	Released
};

USTRUCT(BlueprintType)
struct FPWAbilityInputListener
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag InputTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EPWInputEventType InputEvent = EPWInputEventType::Pressed;
};

USTRUCT()
struct FActiveInputListener
{
	GENERATED_BODY()

	FGameplayAbilitySpecHandle Handle;
	TArray<FPWAbilityInputListener> Listeners;
	bool bExclusive = false;
};
