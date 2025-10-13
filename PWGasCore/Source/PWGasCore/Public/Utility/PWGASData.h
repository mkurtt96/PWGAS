// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "PWGASCoreSettings.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PWGASData.generated.h"

/**
 * 
 */
UCLASS()
class PWGASCORE_API UPWGASData : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UPROPERTY()
	UPWAbilityRegistry* CachedRegistry = nullptr;

public:
	virtual void Initialize(FSubsystemCollectionBase& Coll) override
	{
		if (const auto* Set = UPWGASCoreSettings::Get(); Set && !Set->AbilityRegistry.IsNull())
		{
			CachedRegistry = Set->AbilityRegistry.LoadSynchronous();
		}
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static UPWAbilityRegistry* GetAbilityRegistry(UObject* WorldContextObject);

private:
	static UPWAbilityRegistry* sAbilityRegistry;
};
