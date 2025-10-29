// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "PWDataModule.h"
#include "ScalableFloat.h"
#include "PWAbilityRadiusModule.generated.h"

UCLASS()
class PWGASCORE_API UPWAbilityRadiusModule : public UPWDataModule
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FScalableFloat Radius = 400.f;

	UFUNCTION(BlueprintCallable, Category="Module")
	float GetRadiusValue() const;

	virtual void ExtendParams(USpellParams* Params) const override;
	virtual void Initialize(UPWGameplayAbilityBase* InOwner) override;
};
