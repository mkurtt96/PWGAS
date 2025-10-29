// copyright mkurtt96


#include "GAS/Abilities/Modules/ActionModules/PWAbilityInstantEffectModule.h"

#include "AbilitySystemGlobals.h"
#include "GAS/Abilities/PWGameplayAbilityBase.h"
#include "GAS/Abilities/Modules/ControlModules/PWAbilityTargetingModule.h"
#include "GAS/Abilities/Modules/DataModules/PWAbilityEffectModule.h"

class UPWAbilityTargetingModule;

void UPWAbilityInstantEffectModule::ExecuteImmediateAction()
{
	if (!OwnerAbility)
		return;

	UWorld* World = OwnerAbility->GetWorld();
	if (!World)
		return;

	auto* Targeting = OwnerAbility->GetTargetingModule();
	auto* Effect = OwnerAbility->GetEffectModule();

	if (!Effect)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] InstantEffectAction has no EffectModule!"), *GetNameSafe(this));
		return;
	}

	TArray<AActor*> Targets;
	if (Targeting)
	{
		FPWTargetingResult Result;
		Targeting->ComputeTarget(Result);
		Targets = Result.HitActors;
	}
	else
	{
		if (APawn* OwnerPawn = GetPawn())
			Targets.Add(OwnerPawn);
	}

	UAbilitySystemComponent* SourceASC = OwnerAbility->GetAbilitySystemComponentFromActorInfo();
	if (!SourceASC)
		return;

	for (AActor* Target : Targets)
	{
		if (!Target) continue;
		UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target);
		if (TargetASC)
			Effect->ApplyEffects(SourceASC, TargetASC);
	}

	UE_LOG(LogTemp, Log, TEXT("[%s] Applied instant effect to %d targets."), *GetNameSafe(this), Targets.Num());
}

void UPWAbilityInstantEffectModule::GetRequiredDataModules_Implementation(TArray<TSubclassOf<UPWDataModule>>& OutRequiredModules)
{
	OutRequiredModules.Add(UPWAbilityEffectModule::StaticClass());
}
