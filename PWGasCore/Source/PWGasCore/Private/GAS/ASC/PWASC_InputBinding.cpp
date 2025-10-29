// copyright mkurtt96

#include "GAS/ASC/PWASC_InputBinding.h"

#include "GAS/Interfaces/PWTaggedAbilityInput.h"
#include "GAS/ASC/PWAbilitySystemComponent.h"
#include "GAS/ASC/PWASC_DataManagement.h"
#include "GAS/Data/FPWAbilityInputListener.h"
#include "GAS/Tags/GASCoreTags.h"


FPWASC_InputBinding::FPWASC_InputBinding(UPWAbilitySystemComponent& InASC) : PWASC(InASC)
{
	PWASC.OnAbilityEnded.AddLambda([this](const FAbilityEndedData& AbilityEndedData)
	{
		UnregisterAbilityInputListener(AbilityEndedData.AbilitySpecHandle);
		if (FocusedAbility && FocusedAbility->Handle == AbilityEndedData.AbilitySpecHandle)
			FocusedAbility = nullptr;
	});
}


void FPWASC_InputBinding::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	if (DispatchToExclusiveListeners(InputTag, EPWInputEventType::Pressed, true))
		return;
	DispatchToNonExclusiveListeners(InputTag, EPWInputEventType::Pressed, true);

	if (DispatchToExclusiveListeners(InputTag, EPWInputEventType::Pressed, false))
		return;
	DispatchToNonExclusiveListeners(InputTag, EPWInputEventType::Pressed, false);

	if (InputTag.MatchesTagExact(PWTags::Input::GlobalCancel))
	{
		TryCancelFocusedAbility();
		return;
	}

	FScopedAbilityListLock Lock(PWASC);
	if (FGameplayAbilitySpec* Spec = PWASC.Data().GetSpecOfInput(InputTag))
	{
		if (TryCancelFocusedAbility())
		{
			PWASC.AbilitySpecInputPressed(*Spec);
			if (PWASC.TryActivateAbility(Spec->Handle))
				FocusedAbility = Spec;
		}
	}
}

void FPWASC_InputBinding::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	if (DispatchToExclusiveListeners(InputTag, EPWInputEventType::Released, true))
		return;

	DispatchToNonExclusiveListeners(InputTag, EPWInputEventType::Released, true);

	if (DispatchToExclusiveListeners(InputTag, EPWInputEventType::Released, false))
		return;

	DispatchToNonExclusiveListeners(InputTag, EPWInputEventType::Released, false);

	if (FocusedAbility && FocusedAbility->IsActive())
		PWASC.AbilitySpecInputReleased(*FocusedAbility);
}

void FPWASC_InputBinding::EquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag) const
{
	if (!AbilityTag.IsValid() || !InputTag.IsValid()) return;

	FGameplayTag TargetAbility = PWASC.Data().GetAbilityOfInput(InputTag);

	if (TargetAbility.IsValid() && TargetAbility.MatchesTagExact(AbilityTag)) return;

	SetOrSwapInputOfAbility(AbilityTag, InputTag);
}

void FPWASC_InputBinding::SetOrSwapInputOfAbility(const FGameplayTag& FirstAbilityTag, const FGameplayTag& SecondInputTag) const
{
	FGameplayAbilitySpec* FirstSpec = PWASC.Data().GetSpecOfAbility(FirstAbilityTag);
	if (!FirstSpec) return;

	if (FGameplayAbilitySpec* SecondSpec = PWASC.Data().GetSpecOfInput(SecondInputTag))
	{
		AssignInputToSpec(*SecondSpec, PWASC.Data().GetInputFromSpec(*FirstSpec));
		PWASC.MarkAbilitySpecDirty(*SecondSpec);
		AbilityInputUpdated.Broadcast(SecondInputTag);
	}

	AssignInputToSpec(*FirstSpec, SecondInputTag);
	PWASC.MarkAbilitySpecDirty(*FirstSpec);
	AbilityInputUpdated.Broadcast(FirstAbilityTag);

	AbilityInputsUpdated.Broadcast();
}

void FPWASC_InputBinding::RegisterAbilityInputListener(FGameplayAbilitySpecHandle Handle, const TArray<FPWAbilityInputListener>& Listeners, bool bExclusive)
{
	if (!Handle.IsValid() || Listeners.IsEmpty()) return;

	ActiveInputListeners.RemoveAll([&](const FActiveInputListener& L) { return L.Handle == Handle; });

	FActiveInputListener NewListener;
	NewListener.Handle = Handle;
	NewListener.Listeners = Listeners;
	NewListener.bExclusive = bExclusive;
	ActiveInputListeners.Add(NewListener);
}

void FPWASC_InputBinding::UnregisterAbilityInputListener(FGameplayAbilitySpecHandle Handle)
{
	ActiveInputListeners.RemoveAll([&](const FActiveInputListener& L) { return L.Handle == Handle; });
}

void FPWASC_InputBinding::AssignInputToSpec(FGameplayAbilitySpec& Spec, const FGameplayTag& InputTag) const
{
	ClearInputFromSpec(&Spec);
	Spec.GetDynamicSpecSourceTags().AddTag(InputTag);
}

void FPWASC_InputBinding::ClearInputFromSpec(FGameplayAbilitySpec* Spec) const
{
	const FGameplayTag InputTag = PWASC.Data().GetInputFromSpec(*Spec);
	Spec->GetDynamicSpecSourceTags().RemoveTag(InputTag);
}

bool FPWASC_InputBinding::IsInputUsed(const FGameplayTag& InputTag)
{
	return PWASC.Data().GetSpecOfInput(InputTag) ? true : false;
}

bool FPWASC_InputBinding::TryCancelFocusedAbility()
{
	if (FocusedAbility && PWASC.Data().DoesAbilityHaveTag(FocusedAbility, PWTags::Ability::Control::Cancelable))
	{
		PWASC.CancelAbilityHandle(FocusedAbility->Handle);
		FocusedAbility = nullptr;
		return true;
	}
	return false;
}

bool FPWASC_InputBinding::DispatchToExclusiveListeners(const FGameplayTag& InputTag, EPWInputEventType EventType, bool bFocusedOnly)
{
	if (bFocusedOnly)
	{
		FActiveInputListener* L = ActiveInputListeners.FindByPredicate([&](const FActiveInputListener& L) { return L.Handle == FocusedAbility->Handle; });
		if (!L->bExclusive) return false;

		for (const FPWAbilityInputListener& E : L->Listeners)
		{
			if (E.InputEvent == EventType && E.InputTag.MatchesTagExact(InputTag))
			{
				if (FGameplayAbilitySpec* Spec = PWASC.FindAbilitySpecFromHandle(L->Handle))
				{
					if (Spec->IsActive() && Spec->Ability->GetClass()->ImplementsInterface(UPWTaggedAbilityInput::StaticClass()))
					{
						PWASC.SendTaggedAbilityInput(*Spec, EventType, InputTag);
						return true;
					}
				}
			}
		}
		return false;
	}

	for (const FActiveInputListener& L : ActiveInputListeners)
	{
		if (!L.bExclusive) continue;
		if (L.Handle == FocusedAbility->Handle) continue;

		for (const FPWAbilityInputListener& E : L.Listeners)
		{
			if (E.InputEvent == EventType && E.InputTag.MatchesTagExact(InputTag))
			{
				if (FGameplayAbilitySpec* Spec = PWASC.FindAbilitySpecFromHandle(L.Handle))
				{
					if (Spec->IsActive() && Spec->Ability->GetClass()->ImplementsInterface(UPWTaggedAbilityInput::StaticClass()))
					{
						PWASC.SendTaggedAbilityInput(*Spec, EventType, InputTag);
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool FPWASC_InputBinding::DispatchToNonExclusiveListeners(const FGameplayTag& InputTag, EPWInputEventType EventType, bool bFocusedOnly)
{
	bool bHandled = false;

	if (bFocusedOnly)
	{
		FActiveInputListener* L = ActiveInputListeners.FindByPredicate([&](const FActiveInputListener& L) { return L.Handle == FocusedAbility->Handle; });
		if (L->bExclusive) bHandled = false;;

		for (const FPWAbilityInputListener& E : L->Listeners)
		{
			if (E.InputEvent == EventType && E.InputTag.MatchesTagExact(InputTag))
			{
				if (FGameplayAbilitySpec* Spec = PWASC.FindAbilitySpecFromHandle(L->Handle))
				{
					if (Spec->IsActive() && Spec->Ability->GetClass()->ImplementsInterface(UPWTaggedAbilityInput::StaticClass()))
					{
						PWASC.SendTaggedAbilityInput(*Spec, EventType, InputTag);
						bHandled = true;
					}
				}
			}
		}
		return bHandled;
	}

	for (const FActiveInputListener& L : ActiveInputListeners)
	{
		if (L.bExclusive) continue;
		if (L.Handle == FocusedAbility->Handle) continue;

		for (const FPWAbilityInputListener& E : L.Listeners)
		{
			if (E.InputEvent == EventType && E.InputTag.MatchesTagExact(InputTag))
			{
				if (FGameplayAbilitySpec* Spec = PWASC.FindAbilitySpecFromHandle(L.Handle))
				{
					if (Spec->IsActive() && Spec->Ability->GetClass()->ImplementsInterface(UPWTaggedAbilityInput::StaticClass()))
					{
						PWASC.SendTaggedAbilityInput(*Spec, EventType, InputTag);
						bHandled = true;
					}
				}
			}
		}
	}
	return bHandled;
}

bool FPWASC_InputBinding::ListenerMatchesTagAndEvent(const FActiveInputListener& L, const FGameplayTag& InputTag, EPWInputEventType EventType)
{
	for (const FPWAbilityInputListener& E : L.Listeners)
	{
		if (E.InputEvent == EventType && E.InputTag.MatchesTagExact(InputTag))
			return true;
	}
	return false;
}
