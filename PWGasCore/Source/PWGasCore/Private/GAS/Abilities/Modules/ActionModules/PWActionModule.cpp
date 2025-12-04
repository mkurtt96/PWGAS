// copyright mkurtt96


#include "GAS/Abilities/Modules/ActionModules/PWActionModule.h"

#include "GAS/Abilities/PWModularGameplayAbility.h"


USpellParams* UPWActionModule::MakeSpellParams_Implementation(const FPWTargetingResult& Target)
{
	return nullptr;
}

void UPWActionModule::ExtendParamsWithData(USpellParams* Params) const
{
	for (TObjectPtr Data : OwnerAbility->DataModules)
	{
		Data.Get()->ExtendParams(Params);
	}
}
