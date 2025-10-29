// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagsSettings.h"
#include "Ability/SpellParams.h"
#include "GAS/Data/FPWAbilityInputListener.h"
#include "GAS/Interfaces/PWTaggedAbilityInput.h"
#include "Modules/PWAbilityModule.h"
#include "Modules/ControlModules/PWControlModule.h"
#include "Modules/DataModules/PWDataModule.h"
#include "Targeting/Data/PWTargetingData.h"
#include "PWGameplayAbilityBase.generated.h"

class UPWAbilityMultiActorModule;
class UPWAbilityProjectileModule;
class UPWAbilityMultiProjectileModule;
class UPWAbilityRadiusModule;
class UPWActionModule;
class UPWAbilityActorModule;
class UPWAbilityPrecastModule;
class UPWAbilityEffectModule;
class UPWAbilityAuraModule;
class UPWAbilityTargetingModule;
class UPWAbilityRangeModule;

UCLASS(Blueprintable)
class PWGASCORE_API UPWGameplayAbilityBase : public UGameplayAbility, public IPWTaggedAbilityInput
{
	GENERATED_BODY()

public:
	// == Ability / Config == //
	// ====================== //

	UPROPERTY(EditDefaultsOnly, Category="Ability")
	FGameplayTag AbilityTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ability|Data")
	FScalableFloat Cooldown;

	USpellParams* MakeSpellParams();

	// == Modules  == //
	// ============== //

	UPROPERTY(EditDefaultsOnly, Instanced, Category="Ability|Modules")
	TObjectPtr<UPWActionModule> ActionModule;
	UPROPERTY(EditDefaultsOnly, Instanced, Category="Ability|Modules")
	TArray<TObjectPtr<UPWDataModule>> DataModules;
	UPROPERTY(EditDefaultsOnly, Instanced, Category="Ability|Modules")
	TArray<TObjectPtr<UPWControlModule>> ControlModules;

	template <typename T>
	T* GetActionModule() const
	{
		if (DataModules.Num() == 0)
			return nullptr;

		return Cast<T>(ActionModule);
	}

	template <typename T>
	T* GetDataModule() const
	{
		for (UPWDataModule* Mod : DataModules)
			if (Mod && Mod->IsA(T::StaticClass()))
				return Cast<T>(Mod);

		return nullptr;
	}

	template <typename T>
	T* GetControlModule() const
	{
		for (UPWControlModule* Mod : ControlModules)
			if (Mod && Mod->IsA(T::StaticClass()))
				return Cast<T>(Mod);

		return nullptr;
	}

	void ForEachModule(TFunctionRef<void(UPWAbilityModule*)> Callback) const;

	UFUNCTION(BlueprintPure, Category="Modules")
	UPWAbilityActorModule* GetActorModule() const;
	UFUNCTION(BlueprintPure, Category="Modules")
	UPWAbilityMultiActorModule* GetMultiActorModule() const;
	UFUNCTION(BlueprintPure, Category="Modules")
	UPWAbilityProjectileModule* GetProjectileModule() const;
	UFUNCTION(BlueprintPure, Category="Modules")
	UPWAbilityMultiProjectileModule* GetMultiProjectileModule() const;
	UFUNCTION(BlueprintPure, Category="Modules")
	UPWAbilityPrecastModule* GetPrecastModule() const;
	UFUNCTION(BlueprintPure, Category="Modules")
	UPWAbilityEffectModule* GetEffectModule() const;
	UFUNCTION(BlueprintPure, Category="Modules")
	UPWAbilityAuraModule* GetAuraModule() const;
	UFUNCTION(BlueprintPure, Category="Modules")
	UPWAbilityTargetingModule* GetTargetingModule() const;
	UFUNCTION(BlueprintPure, Category="Modules")
	UPWAbilityRangeModule* GetRangeModule() const;
	UFUNCTION(BlueprintPure, Category="Modules")
	UPWAbilityRadiusModule* GetRadiusModule() const;

	// == Gameplay Targeting  == //
	// ========================= //

	UFUNCTION(BlueprintCallable, Category="Targeting")
	FPWTargetingResult ComputeTargetOnce() const;

	// Build TargetData from targeting result
	UFUNCTION(BlueprintCallable, Category="Targeting")
	FGameplayAbilityTargetDataHandle MakeTargetDataFromResult(const FPWTargetingResult& Result) const;

	// Lightweight extractor for a single TargetData entry
	FVector ExtractLocation(const FGameplayAbilityTargetData* TD) const;

	// == Cooldown  == //
	// =============== //

	virtual float GetCooldown(int32 Level) const;

	// == Visual/Audio/Anim  == //
	// ======================== //

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation")
	FGameplayTag AnimationTag;

	// Lightweight extractor for a single TargetData entry
	UFUNCTION(BlueprintPure, Category="Animation")
	void GetAnimMontageFromActor(UAnimMontage*& OutMontage, float& OutAnimRate) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Audio")
	FGameplayTag StartSoundCue;

	// == IPWTaggedAbilityInput  == //
	// ============================ //

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability|Input")
	TMap<EPWInputEventType, FGameplayTagContainer> InputTagBindings;

	UFUNCTION(BlueprintCallable)
	virtual void HandleTaggedAbilityInput_Implementation(const EPWInputEventType& InputType, const FGameplayTag& InputTag) override;
	UFUNCTION(BlueprintCallable)
	virtual void GetHandledInputTags_Implementation(TArray<FGameplayTag>& OutTags) const override;
	/**
	 * Register this ability for the given tags. All registries are cleared on EndAbility. 
	 * @param Listeners
	 * @param Listeners
	 * @param bExclusive sets ability as the sole listener while it's active. it will prevent other abilities from listening inputs until this one is resolved.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void RegisterAbilityInputListener_Implementation(const TArray<FPWAbilityInputListener>& Listeners, bool bExclusive = false) override;
	UFUNCTION(BlueprintCallable)
	virtual void UnregisterAbilityInputListener_Implementation() override;

	// == Utils  == //
	// ============ //

	APlayerController* GetPC() const;
	APawn* GetPawn() const;

protected:
	// == UE Overrides  == //
	// =================== //

	virtual void PostLoad() override;
	virtual FGameplayTagContainer GetCooldownGameplayTags() const;
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	// == InputHandling  == //
	// ==================== //

	UFUNCTION(BlueprintImplementableEvent, Category="Ability|Input")
	void OnInputPressed(FGameplayTag InputTag);

	UFUNCTION(BlueprintImplementableEvent, Category="Ability|Input")
	void OnInputReleased(FGameplayTag InputTag);

	static bool MatchesBinding(const TMap<EPWInputEventType, FGameplayTagContainer>& Map, EPWInputEventType Type, const FGameplayTag& Tag);

	// == Modules  == //
	// ============== //

	void EnsureRequiredDataModules();

private:
	static TMap<UClass*, TArray<UPWAbilityModule*>> CachedByType;


	// == Editor Helpers  == //
	// ===================== //

#if WITH_EDITOR
	void SyncIdentityIntoAssetTags();
	static void AddTagIfMissing(UGameplayTagsSettings* Settings, const FName& TagFName, const FString& Comment);
	static bool EnsureProjectTagExists(const FString& Tag, const FString& Comment = TEXT(""));

public:
	void GenerateIdentityTag();

protected:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& E) override;

public:

protected:
#endif
};
