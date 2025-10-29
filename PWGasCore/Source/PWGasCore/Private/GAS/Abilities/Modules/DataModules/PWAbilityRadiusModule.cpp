// copyright mkurtt96


#include "GAS/Abilities/Modules/DataModules/PWAbilityRadiusModule.h"

#include "Ability/SpellParams.h"
#include "GAS/Abilities/PWGameplayAbilityBase.h"

float UPWAbilityRadiusModule::GetRadiusValue() const
{
	return OwnerAbility ? Radius.GetValueAtLevel(OwnerAbility->GetAbilityLevel()) : Radius.GetValueAtLevel(1);
}

void UPWAbilityRadiusModule::ExtendParams(USpellParams* Params) const
{
	if (!Params) return;

	Params->More->AddFloat("Radius", GetRadiusValue());
}

void UPWAbilityRadiusModule::Initialize(UPWGameplayAbilityBase* InOwner)
{
	Super::Initialize(InOwner);
	
}
