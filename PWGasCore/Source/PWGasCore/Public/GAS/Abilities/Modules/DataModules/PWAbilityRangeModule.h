// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "PWDataModule.h"
#include "ScalableFloat.h"
#include "Targeting/Types/PWTargetPolicyBase.h"
#include "PWAbilityRangeModule.generated.h"

struct FPWTargetingResult;

UCLASS()
class PWGASCORE_API UPWAbilityRangeModule : public UPWDataModule
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly)
	UPWRangePolicy* RangePolicy;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat Range = 1200.f;

	UFUNCTION(BlueprintPure, Category="Module")
	float GetRangeValue() const;

	UFUNCTION(BlueprintCallable, Category="Module")
	void ApplyRangePolicy(AActor* Origin, FPWTargetingResult& InOut) const;

	virtual void Initialize(UPWGameplayAbilityBase* InOwner) override;
	virtual void ExtendParams(USpellParams* Params) const override;
	
};