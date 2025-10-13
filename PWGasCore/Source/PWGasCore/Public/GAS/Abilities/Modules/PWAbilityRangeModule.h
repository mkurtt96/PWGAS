// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "PWAbilityModule.h"
#include "ScalableFloat.h"
#include "PWAbilityRangeModule.generated.h"

/**
 * 
 */
UCLASS()
class PWGASCORE_API UPWAbilityRangeModule : public UPWAbilityModule
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Targeting|Config", meta=(MustImplement="PWRangePolicy"))
	TSubclassOf<UObject> RangePolicyClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Range")
	FScalableFloat Range = 1200.f;

	UFUNCTION(BlueprintPure, Category="Range")
	float GetRangeValue() const;

	UFUNCTION(BlueprintCallable, Category="Range")
	void ApplyRangePolicy(AActor* Origin, FPWTargetingResult& InOut) const;
};