// copyright mkurtt96

#include "GAS/Abilities/Derived/PWGameplayAbility_Confirmed.h"

void UPWGameplayAbility_Confirmed::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	bAwaitingConfirm = true;
	bConfirmed = false;
}

void UPWGameplayAbility_Confirmed::HandleTaggedAbilityInput_Implementation(const EPWInputEventType& InputType, const FGameplayTag& InputTag)
{
	if (bAwaitingConfirm)
	{
		if (MatchesBinding(ConfirmInputTags, InputType, InputTag))
		{
			bConfirmed = true;
			bAwaitingConfirm = false;
			OnConfirm(InputTag);
			return;
		}

		if (MatchesBinding(CancelInputTags, InputType, InputTag))
		{
			bAwaitingConfirm = false;
			OnCancel(InputTag);
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, /*bReplicateEndAbility*/ true, /*bWasCancelled*/ true);
			return;
		}
	}

	if (bConfirmed || bCanUseInputTagBindingsBeforeConfirmation)
	{
		Super::HandleTaggedAbilityInput_Implementation(InputType, InputTag);
	}
}
