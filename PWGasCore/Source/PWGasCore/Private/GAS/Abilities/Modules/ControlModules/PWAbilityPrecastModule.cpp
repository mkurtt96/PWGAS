// copyright mkurtt96


#include "PWGasCore/Public/GAS/Abilities/Modules/ControlModules/PWAbilityPrecastModule.h"

#include "GAS/Abilities/PWGameplayAbilityBase.h"
#include "GAS/Abilities/Modules/ActionModules/PWAbilityProjectileModule.h"
#include "GAS/Abilities/Modules/ControlModules/PWAbilityTargetingModule.h"
#include "GAS/Abilities/Modules/DataModules/PWAbilityRadiusModule.h"
#include "GAS/Abilities/Modules/DataModules/PWAbilityRangeModule.h"
#include "Precast/PWPrecastControllerComponent.h"

void UPWAbilityPrecastModule::StartPrecast()
{
	if (!OwnerAbility) return;
	if (UPWPrecastControllerComponent* C = GetPrecastController())
	{
		UPWTargetingSource* TargetingSource = nullptr;
		UPWRangePolicy* RangePolicy = nullptr;
		UPWTargetResolver* TargetResolver = nullptr;
		FName OriginSocket = NAME_None;
		FPWProjectileSimConfig ProjectileConfig;

		if (const UPWAbilityTargetingModule* TargetingModule = OwnerAbility->GetTargetingModule())
		{
			TargetingSource = TargetingModule->TargetingSource;
			TargetResolver = TargetingModule->TargetResolver;
		}

		if (const UPWAbilityRangeModule* RangeModule = OwnerAbility->GetRangeModule())
		{
			RangePolicy = RangeModule->RangePolicy;
			PreviewRange = RangeModule->GetRangeValue();
		}
		
		if (const UPWAbilityProjectileModule* ProjectileModule = OwnerAbility->GetProjectileModule())
		{
			OriginSocket = ProjectileModule->SpawnSocket;
			ProjectileConfig = ProjectileModule->ProjectileConfig;
		}

		if (const UPWAbilityRadiusModule* RadiusModule = OwnerAbility->GetRadiusModule())
		{
			PreviewRadius = RadiusModule->GetRadiusValue();
		}

		C->StartPrecast(
			TargetingSource,
			RangePolicy,
			TargetResolver,
			Visualizer,
			PreviewRadius,
			PreviewRange,
			OriginSocket,
			ProjectileConfig
		);
	}
}

void UPWAbilityPrecastModule::StopPrecast()
{
	if (UPWPrecastControllerComponent* C = GetPrecastController())
	{
		C->StopPrecast();
		CachedResult = C->GetTargetingResult();
	}
}

UPWPrecastControllerComponent* UPWAbilityPrecastModule::GetPrecastController() const
{
	if (const APlayerController* PC = GetPC())
		return PC->FindComponentByClass<UPWPrecastControllerComponent>();
	return nullptr;
}
