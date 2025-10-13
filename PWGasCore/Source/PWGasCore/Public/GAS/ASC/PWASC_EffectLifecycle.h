// copyright mkurtt96

#pragma once
#include "GameplayTagContainer.h"


class FPWASC_EffectLifecycle 
{
public:
	friend class UPWAbilitySystemComponent;
	explicit FPWASC_EffectLifecycle(UPWAbilitySystemComponent& InASC) : ASC(InASC){}
	virtual ~FPWASC_EffectLifecycle() = default;

	
	void ApplyBuffTag(const FGameplayTag BuffTag) const;
	void RemoveEffectsByTag(const FGameplayTagContainer& TagsToRemove) const;

private:
	UPWAbilitySystemComponent& ASC;
};
