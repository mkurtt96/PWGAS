// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PWTagFunctions.generated.h"

class UGameplayAbility;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class PWGASCORE_API UPWTagFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="GASCore|Tags", meta=(DisplayName="Get Cancelable Tag"))
	static FGameplayTag GetCancelableTag();

	UFUNCTION(BlueprintPure, Category="GASCore|Flow", meta=(DefaultToSelf="Ability"))
	static bool IsAbilityCurrentlyCancelable(UGameplayAbility* Ability);

	UFUNCTION(BlueprintCallable, Category="GASCore|Flow", meta=(DefaultToSelf="Ability"))
	static void SetAbilityCancelable(UGameplayAbility* Ability, bool bIsCancelable);

	UFUNCTION(BlueprintCallable, Category="GASCore|Flow", meta=(DefaultToSelf="Ability"))
	static FGameplayTag GetAbilityCooldownTag(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable, Category="GASCore|Flow", meta=(DefaultToSelf="Ability"))
	static FGameplayTag GetAbilityTagWithSuffix(const FGameplayTag& AbilityTag, const FString& Suffix);

	UFUNCTION(BlueprintCallable, Category="GASCore|Flow", meta=(DefaultToSelf="Ability"))
	static FGameplayTag FindTagWithSuffix(const FGameplayTagContainer& Container, const FString& Suffix);

private:
	static FGameplayAbilitySpec* GetCurrentSpec(const UGameplayAbility* Ability, UAbilitySystemComponent*& OutASC);
};
