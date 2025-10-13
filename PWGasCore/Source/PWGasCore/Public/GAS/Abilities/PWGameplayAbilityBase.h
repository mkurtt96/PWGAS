// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagsSettings.h"
#include "Ability/SpellParams.h"
#include "GameFramework/PlayerState.h"
#include "GAS/Data/FPWAbilityInputListener.h"
#include "GAS/Interfaces/PWTaggedAbilityInput.h"
#include "Targeting/Data/PWTargetingTypes.h"
#include "PWGameplayAbilityBase.generated.h"

class UPWAbilityTargetingModule;
class UPWAbilityRangeModule;
class UPWAbilityPrecastModule;
class UPWAbilityAuraModule;
class UPWAbilityEffectModule;
class UPWAbilityProjectileModule;
class UPWAbilityActorModule;
class UPWAbilityModule;


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

	UPROPERTY(EditDefaultsOnly, Instanced, Category="Ability|Modules")
	TArray<UPWAbilityModule*> AbilityModules;

	UFUNCTION(BlueprintCallable, Category="Ability|Modules")
	UPWAbilityModule* GetModuleByClass(TSubclassOf<UPWAbilityModule> ComponentClass) const;
	
	template <typename T>
	T* GetModule(int32 Index = 0) const
	{
		TArray<T*> Matches;
		for (UPWAbilityModule* Comp : AbilityModules)
			if (T* AsType = Cast<T>(Comp))
				Matches.Add(AsType);

		if (Matches.IsValidIndex(Index))
			return Matches[Index];
		
		UE_LOG(LogTemp, Warning, TEXT("Ability %s: Requested module %s[%d] but only %d exist."), *GetNameSafe(this), *T::StaticClass()->GetName(), Index, Matches.Num());
		return nullptr;
	}

	template <typename T>
	T* MakeSpellParams()
	{
		static_assert(TIsDerivedFrom<T, USpellParamsBase>::IsDerived, "T must derive from USpellParamsBase");
	
		T* Params = NewObject<T>(this);
		Params->SourceAvatar = GetAvatarActorFromActorInfo();
		Params->AbilityLevel = GetAbilityLevel();

		if (const APlayerState* PS = GetPawn() ? GetPawn()->GetPlayerState() : nullptr)
			Params->SourcePlayerUniqueId = PS->GetUniqueId();

		if (!Params->More)
			Params->More = NewObject<UMultiDataArray>(Params);

		return Params;
	}
	
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
	UAnimMontage* GetAnimationMontageFromActor() const;

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

	// == InputHandling  == //
	// ==================== //


	UFUNCTION(BlueprintPure, Category="Modules")
	UPWAbilityActorModule* GetActorModule(int32 Index = 0) const;

	UFUNCTION(BlueprintPure, Category="Modules")
	UPWAbilityProjectileModule* GetProjectileModule(int32 Index = 0) const;

	UFUNCTION(BlueprintPure, Category="Modules")
	UPWAbilityPrecastModule* GetPrecastModule(int32 Index = 0) const;

	UFUNCTION(BlueprintPure, Category="Modules")
	UPWAbilityEffectModule* GetEffectModule(int32 Index = 0) const;

	UFUNCTION(BlueprintPure, Category="Modules")
	UPWAbilityAuraModule* GetAuraModule(int32 Index = 0) const;

	UFUNCTION(BlueprintPure, Category="Modules")
	UPWAbilityTargetingModule* GetTargetingModule(int32 Index = 0) const;

	UFUNCTION(BlueprintPure, Category="Modules")
	UPWAbilityRangeModule* GetRangeModule(int32 Index = 0) const;

private:




	
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
