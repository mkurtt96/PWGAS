// copyright mkurtt96


#include "GAS/AbilityTasks/AbilityTask_WaitInputEvent.h"

#include "AbilitySystemComponent.h"
#include "GAS/Tags/GASCoreTags.h"

UAbilityTask_WaitInputEvent* UAbilityTask_WaitInputEvent::WaitInputEvent(
	UGameplayAbility* OwningAbility, FGameplayTag InputTag, EInputEventType EventType)
{
	UAbilityTask_WaitInputEvent* Task = NewAbilityTask<UAbilityTask_WaitInputEvent>(OwningAbility);
	Task->WatchedInputTag = InputTag;
	Task->WatchedEventType = EventType;
	return Task;
}

void UAbilityTask_WaitInputEvent::Activate()
{
	if (!Ability) return;

	const FGameplayAbilityActorInfo* Info = Ability->GetCurrentActorInfo();
	if (!Info || !Info->AbilitySystemComponent.IsValid()) return;

	UAbilitySystemComponent* ASC = Info->AbilitySystemComponent.Get();

	ASC->GenericGameplayEventCallbacks.FindOrAdd(PWTags::Event::Input).AddUObject(this, &ThisClass::OnGameplayEventReceived);
}

void UAbilityTask_WaitInputEvent::OnGameplayEventReceived(const FGameplayEventData* Payload)
{
	if (!Payload)
		return;

	// Extract stored input tag
	const FGameplayTagContainer& Tags = Payload->InstigatorTags;
	if (!Tags.HasTagExact(WatchedInputTag))
		return;

	const bool bPressed = Payload->EventMagnitude > 0.5f;
	const EInputEventType ReceivedType = bPressed ? EInputEventType::Pressed : EInputEventType::Released;

	if (ReceivedType != WatchedEventType)
		return;

	EventReceived.Broadcast(WatchedInputTag);
	EndTask();
}