// copyright mkurtt96


#include "GAS/ASC/PWASC_AbilityLifecycle.h"

#include "GameplayAbilitySpec.h"
#include "GAS/Abilities/PWAbilityInfo.h"
#include "GAS/Abilities/PWAbilityRegistry.h"
#include "GAS/ASC/PWAbilitySystemComponent.h"
#include "GAS/ASC/PWASC_DataManagement.h"
#include "GAS/Tags/GASCoreTags.h"
#include "Utility/PWGASData.h"

void FPWASC_AbilityLifecycle::AddAbilities(const FGameplayTagContainer& AbilityTags) const
{
	for (const FGameplayTag& Tag : AbilityTags)
	{
		AddAbility(Tag);
	}
	AbilitiesUpdated.Broadcast();
}

void FPWASC_AbilityLifecycle::AddAbility(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag) const
{
	const FPWAbilityInfo AbilityData = UPWGASData::GetAbilityRegistry(ASC.GetWorld())->GetAbilityByTag(AbilityTag);
	if (!AbilityData.IsValid())
		UE_LOG(LogTemp, Warning, TEXT("%s >> AbilityData is invalid!!"), *FString(__FUNCTION__));

	FGameplayAbilitySpec Spec = FGameplayAbilitySpec(AbilityData.AbilityClass, 1);
	FGameplayAbilitySpecHandle SpecHandle = ASC.GiveAbility(Spec);

	if (InputTag.IsValid())
		ASC.Input().EquipAbility(AbilityTag, InputTag);

	bool Activated = ApplyActivationPolicies(SpecHandle, PWTags::Ability::Activation::OnGranted);

	AbilityAdded.Broadcast(AbilityTag);
}

void FPWASC_AbilityLifecycle::RemoveAbility(const FGameplayTag& AbilityTag) const
{
	FGameplayAbilitySpec* Spec = ASC.Data().GetSpecOfAbility(AbilityTag);
	if (!Spec) return;

	if (ApplyDeactivationPolicies(*Spec, PWTags::Ability::Deactivation::OnRemoval))
		AbilityEnded.Broadcast(AbilityTag);

	ASC.ClearAbility(Spec->Handle);

	AbilityRemoved.Broadcast(AbilityTag);
}

void FPWASC_AbilityLifecycle::UpgradeAbility(const FGameplayTag& AbilityTag)
{
	FGameplayAbilitySpec* Spec = ASC.Data().GetSpecOfAbility(AbilityTag);
	if (!Spec) return;

	Spec->Level++;

	AbilityUpdated.Broadcast(AbilityTag);
	ASC.MarkAbilitySpecDirty(*Spec);
}

void FPWASC_AbilityLifecycle::DowngradeAbility(const FGameplayTag& AbilityTag)
{
	FGameplayAbilitySpec* Spec = ASC.Data().GetSpecOfAbility(AbilityTag);
	if (!Spec) return;

	Spec->Level--;

	AbilityUpdated.Broadcast(AbilityTag);
	ASC.MarkAbilitySpecDirty(*Spec);
}

void FPWASC_AbilityLifecycle::RemoveGrantedAbility(const FGameplayAbilitySpecHandle& Handle) const
{
	ASC.CancelAbilityHandle(Handle);
	ASC.ClearAbility(Handle);
	ASC.MarkAbilitySpecDirty(*ASC.FindAbilitySpecFromHandle(Handle));
}

bool FPWASC_AbilityLifecycle::HasAbility(const FGameplayTag& AbilityTag) const
{
	return ASC.Data().GetSpecOfAbility(AbilityTag) ? true : false;
}

int32 FPWASC_AbilityLifecycle::GetAbilityLevel(const FGameplayTag& AbilityTag) const
{
	if (const FGameplayAbilitySpec* Spec = ASC.Data().GetSpecOfAbility(AbilityTag))
		return Spec->Level;
	return 0;
}

bool FPWASC_AbilityLifecycle::IsAbilityMaxLevel(const FGameplayTag& AbilityTag) const
{
	const FPWAbilityInfo AbilityData = UPWGASData::GetAbilityRegistry(ASC.GetWorld())->GetAbilityByTag(AbilityTag);
	const FGameplayAbilitySpec* Spec = ASC.Data().GetSpecOfAbility(AbilityTag);
	if (!Spec) return false;
	if (Spec->Level == AbilityData.MaxLevel) return true;
	return false;
}

void FPWASC_AbilityLifecycle::ForEachAbility(const FForEachAbility& Delegate) const
{
	FScopedAbilityListLock ActiveScopeLock(ASC);
	for (const FGameplayAbilitySpec& AbilitySpec : ASC.GetActivatableAbilities())
		if (!Delegate.ExecuteIfBound(AbilitySpec))
			UE_LOG(LogTemp, Warning, TEXT("Failed to execute delegate"));
}

bool FPWASC_AbilityLifecycle::ApplyActivationPolicies(const FGameplayAbilitySpecHandle& SpecHandle, const FGameplayTag& EventTag) const
{
	FGameplayAbilitySpec* AbilitySpec = ASC.FindAbilitySpecFromHandle(SpecHandle);
	if (AbilitySpec && AbilitySpec->Ability && AbilitySpec->Ability->GetAssetTags().HasAnyExact(EventTag.GetSingleTagContainer()))
	{
		if (!AbilitySpec->IsActive())
		{
			ASC.TryActivateAbility(SpecHandle);
			return true;
		}
	}
	return false;
}

bool FPWASC_AbilityLifecycle::ApplyActivationPolicies(const FGameplayAbilitySpec& AbilitySpec, const FGameplayTag& EventTag) const
{
	if (AbilitySpec.Ability && AbilitySpec.Ability->GetAssetTags().HasAnyExact(EventTag.GetSingleTagContainer()))
	{
		if (!AbilitySpec.IsActive())
		{
			ASC.TryActivateAbility(AbilitySpec.Handle);
			return true;
		}
	}
	return false;
}

void FPWASC_AbilityLifecycle::ApplyActivationPolicies(const FGameplayTag& EventTag) const
{
	for (FGameplayAbilitySpec* Spec : ASC.Data().GetSpecsWithTags(EventTag.GetSingleTagContainer(), /*exact*/true))
	{
		if (!Spec->IsActive())
		{
			ASC.TryActivateAbility(Spec->Handle);
		}
	}
}

bool FPWASC_AbilityLifecycle::ApplyDeactivationPolicies(const FGameplayAbilitySpecHandle& SpecHandle, const FGameplayTag& EventTag) const
{
	FGameplayAbilitySpec* AbilitySpec = ASC.FindAbilitySpecFromHandle(SpecHandle);
	if (AbilitySpec->Ability && AbilitySpec->Ability->GetAssetTags().HasAnyExact(EventTag.GetSingleTagContainer()))
	{
		if (AbilitySpec->IsActive())
		{
			ASC.CancelAbilityHandle(SpecHandle);
			return true;
		}
	}
	return false;
}

bool FPWASC_AbilityLifecycle::ApplyDeactivationPolicies(const FGameplayAbilitySpec& AbilitySpec, const FGameplayTag& EventTag) const
{
	if (AbilitySpec.Ability && AbilitySpec.Ability->GetAssetTags().HasAnyExact(EventTag.GetSingleTagContainer()))
	{
		if (AbilitySpec.IsActive())
		{
			ASC.CancelAbilityHandle(AbilitySpec.Handle);
			return true;
		}
	}
	return false;
}

void FPWASC_AbilityLifecycle::ApplyDeactivationPolicies(const FGameplayTag& EventTag) const
{
	// end abilities that have this deactivation policy
	for (FGameplayAbilitySpec* Spec : ASC.Data().GetSpecsWithTags(EventTag.GetSingleTagContainer(), /*exact*/true))
	{
		if (Spec->IsActive())
		{
			ASC.CancelAbilityHandle(Spec->Handle);
		}
	}
}
