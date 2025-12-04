// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/PWModularGameplayAbility.h"
#include "PWGameplayAbility_Confirmed.generated.h"


enum class EInputEventType : uint8;
/**
 * 
 */
UCLASS()
class PWGASCORE_API UPWGameplayAbility_Confirmed : public UPWModularGameplayAbility
{
	GENERATED_BODY()

public:

protected:
	UPROPERTY(BlueprintReadOnly)
	bool bAwaitingConfirmation = false;

	UPROPERTY(BlueprintReadOnly)
	bool bConfirmed = false;

	UPROPERTY(EditDefaultsOnly, meta=(GameplayTagFilter = "Input"))
	TMap<EInputEventType, FGameplayTagContainer> ConfirmInputs;

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintImplementableEvent, Category="Ability|Confirm")
	void OnConfirmed(FGameplayTag InputTag);
	UFUNCTION()
	void HandleConfirmInput(const FGameplayTag& InputTag);

	void SetAwaitingConfirmation(bool bCond);
};
