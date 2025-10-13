// copyright mkurtt96


#include "GAS/Tags/PWTagFunctions.h"

#include "AbilitySystemComponent.h"
#include "GAS/Tags/GASCoreTags.h"

FGameplayTag UPWTagFunctions::GetCancelableTag()
{
	return PWTags::Ability::Control::Cancelable;
}

bool UPWTagFunctions::IsAbilityCurrentlyCancelable(UGameplayAbility* Ability)
{
	UAbilitySystemComponent* ASC = nullptr;
	if (FGameplayAbilitySpec* Spec = GetCurrentSpec(Ability, ASC))
	{
		return Spec->GetDynamicSpecSourceTags().HasTag(GetCancelableTag());
	}
	return false;
}

void UPWTagFunctions::SetAbilityCancelable(UGameplayAbility* Ability, bool bIsCancelable)
{
	UAbilitySystemComponent* ASC;
	if (FGameplayAbilitySpec* Spec = GetCurrentSpec(Ability, ASC))
	{
		const FGameplayTag Tag = GetCancelableTag();
		if (bIsCancelable) { Spec->GetDynamicSpecSourceTags().AddTag(Tag); }
		else { Spec->GetDynamicSpecSourceTags().RemoveTag(Tag); }

		if (ASC) { ASC->MarkAbilitySpecDirty(*Spec); } // ensure replication/update
	}
}

FGameplayTag UPWTagFunctions::GetAbilityCooldownTag(const FGameplayTag& AbilityTag)
{
	if (AbilityTag.MatchesTag(PWTags::Ability::Spell::Root))
		return PWTags::Ability::Spell::GetCooldownTag(AbilityTag);
	return FGameplayTag();
}

FGameplayTag UPWTagFunctions::GetAbilityTagWithSuffix(const FGameplayTag& AbilityTag, const FString& Suffix)
{
	if (AbilityTag.MatchesTag(PWTags::Ability::Spell::Root))
	{
		FString FullString = AbilityTag.ToString() + TEXT(".") + Suffix;
		return FGameplayTag::RequestGameplayTag(*FullString, false);
	}
	return FGameplayTag();
}

FGameplayTag UPWTagFunctions::FindTagWithSuffix(const FGameplayTagContainer& Container, const FString& Suffix)
{
	for (const FGameplayTag& Tag : Container)
		if (Tag.ToString().EndsWith(Suffix))
			return Tag;
	return FGameplayTag();
}

FGameplayAbilitySpec* UPWTagFunctions::GetCurrentSpec(const UGameplayAbility* Ability, UAbilitySystemComponent*& OutASC)
{
	OutASC = nullptr;
	if (!Ability) return nullptr;

	const FGameplayAbilityActorInfo* Info = Ability->GetCurrentActorInfo();
	if (!Info) return nullptr;

	OutASC = Info->AbilitySystemComponent.Get();
	if (!OutASC) return nullptr;

	const FGameplayAbilitySpecHandle Handle = Ability->GetCurrentAbilitySpecHandle();
	if (!Handle.IsValid()) return nullptr;

	return OutASC->FindAbilitySpecFromHandle(Handle);
}
