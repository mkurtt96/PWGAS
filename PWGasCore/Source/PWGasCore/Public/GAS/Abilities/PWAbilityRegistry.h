// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "PWAbilityRegistry.generated.h"

class UAbilityDataAsset;
struct FPWAbilityInfo;

UCLASS()
class PWGASCORE_API UPWAbilityRegistry : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UAbilityDataAsset*> Abilities;
	
	UFUNCTION(BlueprintCallable)
	FPWAbilityInfo GetAbilityByTag(const FGameplayTag& AbilityTag);
};
