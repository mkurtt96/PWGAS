// copyright mkurtt96


#include "PWGasCore/Public/GAS/Abilities/Modules/PWAbilityProjectileModule.h"
#include "GAS/Abilities/PWGameplayAbilityBase.h"
#include "GAS/Abilities/Modules/PWAbilityRangeModule.h"


USpellParamsBase* UPWAbilityProjectileModule::MakeSpellParams(const FPWTargetingResult& Target) const
{
    if (!OwnerAbility)
        return nullptr;

    UProjectileSpellParams* Params = OwnerAbility->MakeSpellParams<UProjectileSpellParams>();
    Params->InitialSpeed = ProjectileConfig.InitialSpeed;
    Params->GravityZOverride = ProjectileConfig.GravityZOverride;
    Params->ProjectileRadius = ProjectileConfig.ProjectileRadius;
	Params->TargetCollisionMask = TargetCollisionMask;
	Params->TargetEffectMask = TargetEffectMask;

	if (const UPWAbilityRangeModule* RangeModule = GetLinkedModule<UPWAbilityRangeModule>())
		Params->Range = RangeModule->GetRangeValue();

    return Params;
}