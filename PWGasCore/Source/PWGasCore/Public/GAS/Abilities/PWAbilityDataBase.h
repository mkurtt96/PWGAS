#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PWAbilityDataBase.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FPWAbilityDataBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FGameplayTag Tag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FGameplayTag CooldownTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TSubclassOf<UGameplayAbility> AbilityClass = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	int32 CurrentLevel = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	int32 MaxLevel = 1;

	
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag();
	UPROPERTY(BlueprintReadOnly)
	bool bToBeRemoved = false;
	
public:
	bool IsValid() const
	{
		return Tag.IsValid();
	}
};
