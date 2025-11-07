// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "PWActionModule.h"
#include "PWAbilityInstantEffectModule.generated.h"

/**
 * 
 */
UCLASS()
class PWGASCORE_API UPWAbilityInstantEffectModule : public UPWActionModule
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Module")
	virtual void ExecuteImmediateAction();
	
	virtual void GetRequiredDataModules_Implementation(TArray<TSubclassOf<UPWDataModule>>& OutRequiredModules) override;
	
};
