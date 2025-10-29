// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "PWControlModule.h"
#include "PWAbilityTargetingModule.generated.h"

class UPWTargetingSource;
class UPWTargetResolver;
struct FPWTargetingResult;
/**
 * 
 */
UCLASS()
class PWGASCORE_API UPWAbilityTargetingModule : public UPWControlModule
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly)
	TObjectPtr<UPWTargetingSource> TargetingSource;
	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly)
	TObjectPtr<UPWTargetResolver> TargetResolver;
	
	UFUNCTION(BlueprintCallable, Category="Module")
	bool ComputeTarget(FPWTargetingResult& Out);

	virtual void GetRequiredDataModules_Implementation(TArray<TSubclassOf<UPWDataModule>>& OutRequiredModules) override;
};
