// copyright mkurtt96


#include "Utility/PWGASData.h"

#include "GAS/Abilities/PWAbilityRegistry.h"
#include "Kismet/GameplayStatics.h"

UPWAbilityRegistry* UPWGASData::sAbilityRegistry = nullptr;

UPWAbilityRegistry* UPWGASData::GetAbilityRegistry(UObject* WorldContextObject)
{
	if (!sAbilityRegistry)
	sAbilityRegistry = UGameplayStatics::GetGameInstance(WorldContextObject)->GetSubsystem<UPWGASData>()->CachedRegistry;
	check(sAbilityRegistry);
	return sAbilityRegistry;
}
