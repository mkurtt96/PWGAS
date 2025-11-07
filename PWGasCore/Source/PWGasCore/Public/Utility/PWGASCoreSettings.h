// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "GAS/Abilities/PWAbilityRegistry.h"
#include "PWGASCoreSettings.generated.h"
/**
 * 
 */
UCLASS(config=Game, defaultconfig)
class PWGASCORE_API UPWGASCoreSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
    static const UPWGASCoreSettings* Get(){ return GetDefault<UPWGASCoreSettings>(); }
	
	UPROPERTY(EditAnywhere, config, Category="Registry")
	TSoftObjectPtr<UPWAbilityRegistry> AbilityRegistry;
};
