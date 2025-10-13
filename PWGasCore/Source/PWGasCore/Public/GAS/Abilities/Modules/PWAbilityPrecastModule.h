// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "PWAbilityModule.h"
#include "Targeting/Data/PWTargetingTypes.h"
#include "PWAbilityPrecastModule.generated.h"


class UPWPrecastControllerComponent;

UCLASS(Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class PWGASCORE_API UPWAbilityPrecastModule : public UPWAbilityModule
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="")
	TSubclassOf<UObject> VisualizerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="")
	float PreviewRadius = 150.f;
	
	UFUNCTION(BlueprintCallable, Category="Precast")
	void StartPrecast();

	UFUNCTION(BlueprintCallable, Category="Precast")
	void StopPrecast();

	UFUNCTION(BlueprintPure, Category="Precast")
	FPWTargetingResult GetPrecastResult() const { return CachedResult; }

protected:
	UPROPERTY()
	FPWTargetingResult CachedResult;

private:
	UPWPrecastControllerComponent* GetPrecastController() const;
};
