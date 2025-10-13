// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "PWASC_AbilityLifecycle.h"
#include "PWASC_DataManagement.h"
#include "PWASC_EffectLifecycle.h"
#include "PWASC_InputBinding.h"
#include "PWAbilitySystemComponent.generated.h"

enum class EPWInputEventType : uint8;

DECLARE_MULTICAST_DELEGATE_TwoParams(FAnyGameplayTagChangedDelegate, FGameplayTag& /*Tag*/, int /*Count*/);

UCLASS(ClassGroup=(GASCore), meta=(BlueprintSpawnableComponent))
class PWGASCORE_API UPWAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UPWAbilitySystemComponent();

	FAnyGameplayTagChangedDelegate AnyGameplayTagChanged; //server and client

	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	virtual void BeginPlay() override;
	virtual void OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec) override;
	virtual void OnRep_ActivateAbilities() override;

	FPWASC_DataManagement& Data() const { return *DataManagement; }
	FPWASC_AbilityLifecycle& Abilities() const { return *AbilityLifecycle; }
	FPWASC_EffectLifecycle& Effects() const { return *EffectLifecycle; }
	FPWASC_InputBinding& Input() const { return *InputBinding; }

	UFUNCTION(BlueprintCallable, Category="GASCore|Cooldown")
	bool GetCooldownRemainingForTag(const FGameplayTag& CooldownTag, float& TimeRemaining, float& CooldownDuration) const;
	UFUNCTION(BlueprintCallable, Category="GASCore|Cooldown")
	bool GetCooldownRemainingForAbilityTag(const FGameplayTag& AbilityTag, float& TimeRemaining, float& TotalDuration) const;
	UFUNCTION(BlueprintCallable, Category="GASCore|Cooldown")
	int32 ClearCooldownByTag(const FGameplayTag& CooldownTag);
	UFUNCTION(BlueprintCallable, Category="GASCore|Cooldown")
	int32 ClearCooldownForAbilityTag(const FGameplayTag& AbilityTag);
	UFUNCTION(BlueprintCallable, Category="GASCore|Cooldown")
	bool AdjustCooldownForTag(const FGameplayTag& AbilityTag, float SecondsToReduce);
	UFUNCTION(BlueprintCallable, Category="GASCore|Cooldown")
	bool AdjustCooldownForAbilityTag(const FGameplayTag& AbilityTag, float SecondsToReduce);

	UFUNCTION()
	void SendTaggedAbilityInput(FGameplayAbilitySpec AbilitySpec, EPWInputEventType InputType, FGameplayTag InputTag);

protected:
	virtual TUniquePtr<FPWASC_DataManagement> CreateDataManagement();
	virtual TUniquePtr<FPWASC_AbilityLifecycle> CreateAbilityLifecycle();
	virtual TUniquePtr<FPWASC_EffectLifecycle> CreateEffectLifecycle();
	virtual TUniquePtr<FPWASC_InputBinding> CreateInputBinding();

	UFUNCTION(Server, Reliable)
	void ServerSendTaggedAbilityInput(FGameplayAbilitySpecHandle AbilityHandle, FPredictionKey PredictionKey, EPWInputEventType InputType, FGameplayTag InputTag);

private:
	TUniquePtr<FPWASC_DataManagement> DataManagement;
	TUniquePtr<FPWASC_AbilityLifecycle> AbilityLifecycle;
	TUniquePtr<FPWASC_EffectLifecycle> EffectLifecycle;
	TUniquePtr<FPWASC_InputBinding> InputBinding;
};
