﻿

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/PWGameplayAbilityBase.h"
#include "PWGameplayAbility_Instant.generated.h"


/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PWGASCORE_API UPWGameplayAbility_Instant : public UPWGameplayAbilityBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability|Commit")
	bool bAutoCommit = true;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
