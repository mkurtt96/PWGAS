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
	if (InputType != EPWInputEventType::Pressed)
		return;
	
	if (bAwaitingConfirm)
	{
		if (ConfirmInputTags.HasTagExact(InputTag))
		{
			bConfirmed = true;
			bAwaitingConfirm = false;
			OnConfirm(InputTag);
			return;
		}
		if (CancelInputTags.HasTagExact(InputTag))
		{
			bAwaitingConfirm = false;
			OnCancel(InputTag);
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
			return;
		}
	}

	if (bConfirmed || bCanUseInputTagBindingsBeforeConfirmation)
	{
		Super::HandleTaggedAbilityInput_Implementation(InputType, InputTag);
	}
}
