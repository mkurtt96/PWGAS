// copyright mkurtt96


#include "GAS/Abilities/PWAbilityRegistry.h"

#include "GAS/Abilities/PWAbilityDataBase.h"

FPWAbilityDataBase UPWAbilityRegistry::GetAbilityByTag(const FGameplayTag& AbilityTag)
{
	for (const auto Ability : Abilities)
	{
		if (Ability.Tag.MatchesTagExact(AbilityTag))
		{
			return Ability;
		}
	}
	return FPWAbilityDataBase();
}
