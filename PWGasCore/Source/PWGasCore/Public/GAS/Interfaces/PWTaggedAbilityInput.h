// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "PWTaggedAbilityInput.generated.h"

struct FPWAbilityInputListener;
enum class EPWInputEventType : uint8;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UPWTaggedAbilityInput : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PWGASCORE_API IPWTaggedAbilityInput
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to handle input
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HandleTaggedAbilityInput(const EPWInputEventType& InputType, const FGameplayTag& InputTag);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetHandledInputTags(TArray<FGameplayTag>& OutTags) const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RegisterAbilityInputListener(const TArray<FPWAbilityInputListener>& Listeners, bool bExclusive = false);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UnregisterAbilityInputListener();
};
