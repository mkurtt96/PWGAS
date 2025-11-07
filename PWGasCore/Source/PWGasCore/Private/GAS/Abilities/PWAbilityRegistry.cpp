// copyright mkurtt96


#include "GAS/Abilities/PWAbilityRegistry.h"

#include "GAS/Abilities/AbilityDataAsset.h"
#include "GAS/Abilities/PWAbilityInfo.h"

FPWAbilityInfo UPWAbilityRegistry::GetAbilityByTag(const FGameplayTag& AbilityTag)
{
	for (const auto AbilityDA : Abilities)
	{
		if (AbilityDA->Ability.Tag.MatchesTagExact(AbilityTag))
		{
			return AbilityDA->Ability;
		}
	}
	return FPWAbilityInfo();
}
