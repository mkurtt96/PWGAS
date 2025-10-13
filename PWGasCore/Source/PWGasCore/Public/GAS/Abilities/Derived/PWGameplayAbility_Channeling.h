// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "PWGameplayAbility_Charged.h"
#include "PWGameplayAbility_Channeling.generated.h"

/**
 * 
 */
UCLASS()
class PWGASCORE_API UPWGameplayAbility_Channeling : public UPWGameplayAbility_Charged
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Channel")
	bool bAutoStartChannel = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Channel")
	float ChannelTickInterval = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Channel")
	float MaxChannelDuration = 0.f; // 0 = infinite

	UPROPERTY(BlueprintReadOnly, Category="Channel")
	bool bIsChanneling = false;

	UPROPERTY(BlueprintReadOnly, Category="Channel")
	float CurrentChannelTime = 0.f;

	FTimerHandle ChannelTickTimer;

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
								 const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category="Channel")
	virtual void StartChanneling();

	UFUNCTION(BlueprintCallable, Category="Channel")
	virtual void PauseChanneling();

	UFUNCTION(BlueprintCallable, Category="Channel")
	virtual void StopChanneling(bool bWasCanceled = false);

	UFUNCTION(BlueprintPure, Category="Channel")
	float GetChannelPercentage() const;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="Channel")
	void OnChannelStart();

	UFUNCTION(BlueprintImplementableEvent, Category="Channel")
	void OnChannelTick(float ElapsedTime);

	UFUNCTION(BlueprintImplementableEvent, Category="Channel")
	void OnChannelEnd(bool bWasCanceled);

	void HandleChannelTick();
};
