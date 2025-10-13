// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/PWGameplayAbilityBase.h"
#include "PWGameplayAbility_Confirmed.generated.h"

/**
 * 
 */
UCLASS()
class PWGASCORE_API UPWGameplayAbility_Confirmed : public UPWGameplayAbilityBase
{
	GENERATED_BODY()

public:
	
	/**
	 * Unlike InputTagBindings, these tags are used for initial confirmation.
	 * Until confirmation is done by one of these tags, InputTagBindings tags will not trigger OnInputPressed or OnInputReleased to prevent duplicate events.
	 * This is to streamline the behavior creation. If your ability can be manipulated before it's confirmed, set bCanUseInputTagBindingsBeforeConfirmation to true.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability|Confirm")
	FGameplayTagContainer ConfirmInputTags;

	/**
	 * These tags are used for initial cancellation. i.e precast ability cancellation during precast before the ability is actually casted. 
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability|Confirm")
	FGameplayTagContainer CancelInputTags;

protected:
	UPROPERTY(BlueprintReadOnly)
	bool bAwaitingConfirm = false;

	UPROPERTY(BlueprintReadOnly)
	bool bConfirmed = false;

	/**
	 * Until confirmation is done by one of ConfirmInputTags, InputTagBindings tags will not trigger OnInputPressed or OnInputReleased to prevent duplicate events.
	 * This is to streamline the behavior creation. If your ability can be manipulated before it's confirmed, set bCanUseInputTagBindingsBeforeConfirmation to true.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability|Confirm")
	bool bCanUseInputTagBindingsBeforeConfirmation = false;

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void HandleTaggedAbilityInput_Implementation(const EPWInputEventType& InputType, const FGameplayTag& InputTag) override;

	UFUNCTION(BlueprintImplementableEvent, Category="Ability|Confirm")
	void OnConfirm(FGameplayTag InputTag);

	UFUNCTION(BlueprintImplementableEvent, Category="Ability|Confirm")
	void OnCancel(FGameplayTag InputTag);
};
