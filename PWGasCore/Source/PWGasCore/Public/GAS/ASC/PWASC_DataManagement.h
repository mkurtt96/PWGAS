// copyright mkurtt96

#pragma once
#include "GameplayTagContainer.h"

struct FActiveGameplayEffect;
class UPWAbilitySystemComponent;
struct FGameplayAbilitySpec;

class FPWASC_DataManagement
{
public:
	friend class UPWAbilitySystemComponent;
	explicit FPWASC_DataManagement(UPWAbilitySystemComponent& InASC) : ASC(InASC){}
	virtual ~FPWASC_DataManagement() = default;

	static FGameplayTag GetInputFromSpec(const FGameplayAbilitySpec& Spec);
	static FGameplayTag GetAbilityFromSpec(const FGameplayAbilitySpec& Spec);
	FGameplayAbilitySpec* GetSpecOfAbility(const FGameplayTag& AbilityTag) const;
	FGameplayAbilitySpec* GetSpecOfInput(const FGameplayTag& InputTag) const;
	FGameplayTag GetInputOfAbility(const FGameplayTag& AbilityTag) const;
	FGameplayTag GetAbilityOfInput(const FGameplayTag& InputTag) const;

	const FActiveGameplayEffect* GetSpecOfEffect(const FGameplayTag& EffectTag) const;

	bool DoesAbilityHaveTag(const FGameplayTag& AbilityTag, const FGameplayTagContainer& TagsToCheck, bool bMatchExact = true, bool bIncludeDynamicTags = true) const;
	bool DoesAbilityHaveTag(const FGameplayTag& AbilityTag, const FGameplayTag& TagToCheck, bool bMatchExact = true, bool bIncludeDynamicTags = true) const;
	static bool DoesAbilityHaveTag(const FGameplayAbilitySpec* AbilitySpec, const FGameplayTagContainer& TagsToCheck, bool bMatchExact = true, bool bIncludeDynamicTags = true);
	static bool DoesAbilityHaveTag(const FGameplayAbilitySpec* AbilitySpec, const FGameplayTag& TagToCheck, bool bMatchExact = true, bool bIncludeDynamicTags = true);
	TArray<FGameplayTag> GetAbilitiesWithTags(const FGameplayTagContainer& TagsToCheck, bool bMatchExact = true, bool bIncludeDynamicTags = true) const;
	TArray<FGameplayTag> GetAbilitiesWithTags(const FGameplayTag& TagToCheck, bool bMatchExact = true, bool bIncludeDynamicTags = true) const;
	FGameplayTag GetFirstAbilityWithTag(const FGameplayTagContainer& TagsToCheck, bool bMatchExact = true, bool bIncludeDynamicTags = true) const;
	FGameplayTag GetFirstAbilityWithTag(const FGameplayTag& TagToCheck, bool bMatchExact = true, bool bIncludeDynamicTags = true) const;
	TArray<FGameplayAbilitySpec*> GetSpecsWithTags(const FGameplayTagContainer& TagsToCheck,bool bMatchExact = true,bool bIncludeDynamicTags = true) const;
	TArray<FGameplayAbilitySpec*> GetSpecsWithTags(const FGameplayTag& TagToCheck,bool bMatchExact = true,bool bIncludeDynamicTags = true) const;
	FGameplayAbilitySpec* GetFirstSpecWithTag(const FGameplayTagContainer& TagsToCheck,bool bMatchExact = true,bool bIncludeDynamicTags = true) const;
	FGameplayAbilitySpec* GetFirstSpecWithTag(const FGameplayTag& TagToCheck,bool bMatchExact = true,bool bIncludeDynamicTags = true) const;
	FGameplayAbilitySpec* GetFirstActiveSpecWithTag(const FGameplayTagContainer& TagsToCheck,bool bMatchExact = true,bool bIncludeDynamicTags = true) const;
	FGameplayAbilitySpec* GetFirstActiveSpecWithTag(const FGameplayTag& TagsToCheck,bool bMatchExact = true,bool bIncludeDynamicTags = true) const;
	
	FGameplayAbilitySpec* GetPrecastingSpec() const;
	


private:
	UPWAbilitySystemComponent& ASC;
};
