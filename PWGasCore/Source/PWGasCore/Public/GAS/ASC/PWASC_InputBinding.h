// copyright mkurtt96

#pragma once
#include "GameplayAbilitySpecHandle.h"
#include "GameplayTagContainer.h"

enum class EPWInputEventType : uint8;
struct FPWAbilityInputListener;
struct FActiveInputListener;
struct FGameplayAbilitySpec;

DECLARE_MULTICAST_DELEGATE(FAbilitiesEvent)
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilityEvent, const FGameplayTag&);

class FPWASC_InputBinding
{
public:
	friend class UPWAbilitySystemComponent;
	explicit FPWASC_InputBinding(UPWAbilitySystemComponent& InASC);
	virtual ~FPWASC_InputBinding() = default;
	
	FAbilitiesEvent AbilityInputsUpdated;
	FAbilityEvent AbilityInputUpdated;
	
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void EquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag) const;
	auto SetOrSwapInputOfAbility(const FGameplayTag& FirstAbilityTag, const FGameplayTag& SecondInputTag) const -> void;
	void AssignInputToSpec(FGameplayAbilitySpec& Spec, const FGameplayTag& InputTag) const;
	void ClearInputFromSpec(FGameplayAbilitySpec* Spec) const;
	bool IsInputUsed(const FGameplayTag& InputTag);

	FGameplayAbilitySpec* FocusedAbility;
	TArray<FActiveInputListener> ActiveInputListeners;

	void RegisterAbilityInputListener(FGameplayAbilitySpecHandle Handle, const TArray<FPWAbilityInputListener>& Listeners, bool bExclusive);
	void UnregisterAbilityInputListener(FGameplayAbilitySpecHandle Handle);

private:
	UPWAbilitySystemComponent& PWASC;

	bool TryCancelFocusedAbility();

	bool DispatchToExclusiveListeners(const FGameplayTag& InputTag, EPWInputEventType EventType, bool bFocusedOnly);
	bool DispatchToNonExclusiveListeners(const FGameplayTag& InputTag, EPWInputEventType EventType, bool bFocusedOnly);
	static bool ListenerMatchesTagAndEvent(const FActiveInputListener& L, const FGameplayTag& InputTag, EPWInputEventType EventType);
};


