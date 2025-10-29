// copyright mkurtt96


#include "GAS/Abilities/Modules/DataModules/PWAbilityRangeModule.h"

#include "Debug/Logging.h"
#include "GAS/Abilities/PWGameplayAbilityBase.h"
#include "PWGasCore/Public/Targeting/Types/PWTargetPolicyBase.h"

float UPWAbilityRangeModule::GetRangeValue() const
{
	return OwnerAbility ? Range.GetValueAtLevel(OwnerAbility->GetAbilityLevel()) : Range.GetValueAtLevel(1);
}

void UPWAbilityRangeModule::ApplyRangePolicy(AActor* Origin, FPWTargetingResult& InOut) const
{
	if (!RangePolicy)
	{
		pwlogability(Error, "ApplyRangePolicy failed: RangePolicy is null in %s", *GetNameSafe(this));
		return;
	}

	if (!Origin)
	{
		pwlogability(Warning, "ApplyRangePolicy: Origin actor is null for %s", *GetNameSafe(this));
		return;
	}

	if (!OwnerAbility)
	{
		pwlogability(Error, "ApplyRangePolicy failed: OwnerAbility is null for %s", *GetNameSafe(this));
		return;
	}

	RangePolicy->Initialize(OwnerAbility);

	const float RangeValue = GetRangeValue();
	if (RangeValue <= 0.f)
	{
		pwlogability(Warning, "ApplyRangePolicy: Range value <= 0 in %s", *GetNameSafe(this));
		return;
	}

	RangePolicy->Apply(Origin, RangeValue, InOut);
}

void UPWAbilityRangeModule::Initialize(UPWGameplayAbilityBase* InOwner)
{
	Super::Initialize(InOwner);
	RangePolicy->Initialize(InOwner);
}

void UPWAbilityRangeModule::ExtendParams(USpellParams* Params) const
{
	if (!Params)
		return;

	Params->More->AddFloat("Range", GetRangeValue());
}
