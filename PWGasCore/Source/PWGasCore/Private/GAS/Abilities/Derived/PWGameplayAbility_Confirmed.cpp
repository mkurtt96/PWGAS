// copyright mkurtt96

#include "GAS/Abilities/Derived/PWGameplayAbility_Confirmed.h"

#include "Abilities/Tasks/AbilityTask.h"
#include "GAS/AbilityTasks/AbilityTask_WaitInputEvent.h"
#include "GAS/Tags/GASCoreTags.h"

void UPWGameplayAbility_Confirmed::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	SetAwaitingConfirmation(true);
	bConfirmed = false;

	for (const TPair<EInputEventType, FGameplayTagContainer>& ConfirmInput : ConfirmInputs)
	{
		const EInputEventType EventType = ConfirmInput.Key;
		const FGameplayTagContainer& InputTags = ConfirmInput.Value;

		for (const FGameplayTag& InputTag : InputTags)
		{
			if (!InputTag.IsValid())
				continue;

			UAbilityTask_WaitInputEvent* WaitTask = UAbilityTask_WaitInputEvent::WaitInputEvent(this, InputTag, EventType);
			if (!WaitTask)
				continue;

			WaitTask->EventReceived.AddDynamic(this, &UPWGameplayAbility_Confirmed::HandleConfirmInput);
			WaitTask->ReadyForActivation();
		}
	}
}

void UPWGameplayAbility_Confirmed::SetAwaitingConfirmation(bool bCond)
{
	bAwaitingConfirmation = bCond;
	if (bAwaitingConfirmation)
		GetCurrentAbilitySpec()->GetDynamicSpecSourceTags().AddTag(PWTags::Ability::State::AwaitingConfirmation);
	else
		GetCurrentAbilitySpec()->GetDynamicSpecSourceTags().RemoveTag(PWTags::Ability::State::AwaitingConfirmation);
}

void UPWGameplayAbility_Confirmed::HandleConfirmInput(const FGameplayTag& InputTag)
{
	bConfirmed = true;
	SetAwaitingConfirmation(false);

	OnConfirmed(InputTag);
}