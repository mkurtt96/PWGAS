// copyright mkurtt96

#pragma once
#include "GameplayTagContainer.h"

struct FGameplayAbilitySpec;
struct FGameplayAbilitySpecHandle;
struct FActiveGameplayEffect;

DECLARE_MULTICAST_DELEGATE(FAbilitiesEvent)
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilityEvent, const FGameplayTag&);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);



class FPWASC_AbilityLifecycle
{
public:
	friend class UPWAbilitySystemComponent;
	explicit FPWASC_AbilityLifecycle(UPWAbilitySystemComponent& InASC) : ASC(InASC) {}
	virtual ~FPWASC_AbilityLifecycle() = default;
	
	FAbilitiesEvent AbilitiesUpdated; //server and client 
	FAbilityEvent AbilityAdded; //server only
	FAbilityEvent AbilityRemoved; //server only
	FAbilityEvent AbilityUpdated; //server only
	
	FAbilityEvent AbilityEnded; //server only
	
	/**
	 * Adds multiple abilities, broadcasts Abilities Updated.
	 * Intended to be used to grand startup abilities.
	 * @param AbilityTags 
	 */
	virtual void AddAbilities(const FGameplayTagContainer& AbilityTags) const;
	virtual void AddAbility(const FGameplayTag& AbilityTag, const bool AutoEquip = true) const;
	virtual void RemoveAbility(const FGameplayTag& AbilityTag) const;
	virtual void UpgradeAbility(const FGameplayTag& AbilityTag);
	virtual void DowngradeAbility(const FGameplayTag& AbilityTag);

	void RemoveGrantedAbility(const FGameplayAbilitySpecHandle& Handle) const;

	virtual bool HasAbility(const FGameplayTag& AbilityTag) const;
	virtual int32 GetAbilityLevel(const FGameplayTag& AbilityTag) const;
	virtual bool IsAbilityMaxLevel(const FGameplayTag& AbilityTag) const;

	virtual void ForEachAbility(const FForEachAbility& Delegate) const;
	
	virtual bool ApplyActivationPolicies(const FGameplayAbilitySpecHandle& SpecHandle, const FGameplayTag& EventTag) const;
	virtual bool ApplyActivationPolicies(const FGameplayAbilitySpec& AbilitySpec, const FGameplayTag& EventTag) const;
	virtual void ApplyActivationPolicies(const FGameplayTag& EventTag) const;
	virtual bool ApplyDeactivationPolicies(const FGameplayAbilitySpecHandle& SpecHandle, const FGameplayTag& EventTag) const;
	virtual bool ApplyDeactivationPolicies(const FGameplayAbilitySpec& AbilitySpec, const FGameplayTag& EventTag) const;
	virtual void ApplyDeactivationPolicies(const FGameplayTag& EventTag) const;

private:
	UPWAbilitySystemComponent& ASC;
};
