// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "PWAbilityModule.h"
#include "PWAbilityTargetingModule.generated.h"

struct FPWTargetingResult;
/**
 * 
 */
UCLASS()
class PWGASCORE_API UPWAbilityTargetingModule : public UPWAbilityModule
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Targeting|Config")
	TSubclassOf<UObject> TargetingSourceClass; // implements IPWTargetingSource

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Targeting|Config")
	bool bClampToRange = true;
	
	UFUNCTION(BlueprintCallable, Category="Targeting")
	bool ComputeTarget(FPWTargetingResult& Out);
};
