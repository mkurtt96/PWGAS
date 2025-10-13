// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "PWAbilityRegistry.generated.h"

struct FPWAbilityDataBase;

UCLASS()
class PWGASCORE_API UPWAbilityRegistry : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FPWAbilityDataBase> Abilities;
	
	UFUNCTION(BlueprintCallable)
	FPWAbilityDataBase GetAbilityByTag(const FGameplayTag& AbilityTag);
};
