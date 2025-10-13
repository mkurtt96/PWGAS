// copyright mkurtt96


#include "GAS/Abilities/Modules/PWAbilityRangeModule.h"

#include "GAS/Abilities/PWGameplayAbilityBase.h"
#include "Targeting/Interfaces/PWTargetingInterfaces.h"

float UPWAbilityRangeModule::GetRangeValue() const
{
	return OwnerAbility ? Range.GetValueAtLevel(OwnerAbility->GetAbilityLevel()) : Range.GetValueAtLevel(1);
}

void UPWAbilityRangeModule::ApplyRangePolicy(AActor* Origin, FPWTargetingResult& InOut) const
{
	if (!RangePolicyClass || !Origin) return;

	if (UObject* RangeObj = NewObject<UObject>(const_cast<UPWAbilityRangeModule*>(this), RangePolicyClass))
		if (const IPWRangePolicy* R = Cast<IPWRangePolicy>(RangeObj))
			R->Execute_Apply(RangeObj, Origin, GetRangeValue(), InOut);
}
