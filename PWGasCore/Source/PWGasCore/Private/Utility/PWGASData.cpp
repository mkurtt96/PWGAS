// copyright mkurtt96


#include "Utility/PWGASData.h"

#include "GAS/Abilities/PWAbilityRegistry.h"
#include "Kismet/GameplayStatics.h"

UPWAbilityRegistry* UPWGASData::sAbilityRegistry = nullptr;

UPWAbilityRegistry* UPWGASData::GetAbilityRegistry(UObject* WorldContextObject)
{
	if (!sAbilityRegistry)
	{
		const UPWGASData* GASData = UGameplayStatics::GetGameInstance(WorldContextObject)->GetSubsystem<UPWGASData>();
		if (!GASData)
			UE_LOG(LogTemp, Error, TEXT("%s >> GAS Data for AbilityRegistry is null!!!"), *FString(__FUNCTION__));
		sAbilityRegistry = GASData->CachedRegistry;
	}
	check(sAbilityRegistry);
	return sAbilityRegistry;
}
