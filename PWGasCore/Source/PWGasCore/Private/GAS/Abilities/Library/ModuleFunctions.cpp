// copyright mkurtt96


#include "GAS/Abilities/Library/ModuleFunctions.h"

#include "Ability/SpellParams.h"

UEffectSpellParamsExtension* UModuleFunctions::GetEffectParams(const USpellParams* Params)
{
	return Params ? Params->FindExtension<UEffectSpellParamsExtension>() : nullptr;
}



float UModuleFunctions::GetRange(const USpellParams* Params)
{
	return Params ? Params->More->GetFloat("Range") : 0.f;
}

float UModuleFunctions::GetRadius(const USpellParams* Params)
{
	return Params ? Params->More->GetFloat("Radius") : 0.f;
}
