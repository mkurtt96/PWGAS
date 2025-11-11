// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagsSettings.h"
#include "Ability/SpellParams.h"
#include "Modules/PWAbilityModule.h"
#include "Modules/ControlModules/PWControlModule.h"
#include "Modules/DataModules/PWDataModule.h"
#include "Targeting/Data/PWTargetingData.h"
#include "PWGameplayAbilityBase.generated.h"

class UPWAbilityInstantEffectModule;
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
class PWGASCORE_API UPWGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

public:
	// == Ability / Config == //
	// ====================== //

	UPROPERTY(EditDefaultsOnly, Category="Ability", meta=(GameplayTagFilter = "Ability.Skill"))
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

	void ForEachModule(TFunctionRef<void(UPWAbilityModule*)> Callback);

	UFUNCTION(BlueprintPure, Category="Modules")
	UPWAbilityInstantEffectModule* GetInstantEffectModule() const;
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation", meta=(GameplayTagFilter = "AnimMontage"))
	TArray<FGameplayTag> AnimationTags;

	// Lightweight extractor for a single TargetData entry
	UFUNCTION(BlueprintPure, Category="Animation")
	void GetAnimMontageFromActor(UAnimMontage*& OutMontage, float& OutAnimRate, int Index = 0) const;
	
	UFUNCTION(BlueprintPure, Category="Animation")
	UAnimInstance* GetAnimInstanceFromActor() const;
	UFUNCTION(BlueprintPure)
	FGameplayTag GetActivationInputTag() const;
	
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
