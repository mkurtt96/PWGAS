// copyright mkurtt96


#include "PWGasCore/Public/GAS/Abilities/Modules/PWAbilityPrecastModule.h"

#include "GAS/Abilities/PWGameplayAbilityBase.h"
#include "GAS/Abilities/Modules/PWAbilityProjectileModule.h"
#include "GAS/Abilities/Modules/PWAbilityRangeModule.h"
#include "GAS/Abilities/Modules/PWAbilityTargetingModule.h"
#include "Precast/PWPrecastControllerComponent.h"

void UPWAbilityPrecastModule::StartPrecast()
{
	if (!OwnerAbility) return;
	if (UPWPrecastControllerComponent* C = GetPrecastController())
	{
		TSubclassOf<UObject> TargetingClass = nullptr;
		TSubclassOf<UObject> RangePolicyClass = nullptr;
		FName OriginSocket = NAME_None;
		FPWProjectileSimConfig ProjectileConfig;

		if (const UPWAbilityTargetingModule* TargetingModule = GetLinkedModule<UPWAbilityTargetingModule>())
			TargetingClass = TargetingModule->TargetingSourceClass;

		if (const UPWAbilityRangeModule* RangeModule = GetLinkedModule<UPWAbilityRangeModule>())
			RangePolicyClass = RangeModule->RangePolicyClass;

		if (const UPWAbilityProjectileModule* ProjectileModule = GetLinkedModule<UPWAbilityProjectileModule>())
		{
			OriginSocket = ProjectileModule->SpawnSocket;
			ProjectileConfig = ProjectileModule->ProjectileConfig;
		}

		C->StartPrecast(
			TargetingClass,
			RangePolicyClass,
			VisualizerClass,
			PreviewRadius,
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
