// copyright mkurtt96


#include "GAS/Abilities/Modules/ControlModules/PWAbilityTargetingModule.h"

#include "Debug/Logging.h"
#include "GAS/Abilities/PWGameplayAbilityBase.h"
#include "GAS/Abilities/Modules/DataModules/PWAbilityRangeModule.h"
#include "PWGasCore/Public/Targeting/Types/PWTargetPolicyBase.h"

bool UPWAbilityTargetingModule::ComputeTarget(FPWTargetingResult& Out)
{
	if (!OwnerAbility)
	{
		pwlogability(Error, "UPWAbilityTargetingModule::ComputeTarget failed: OwnerAbility is null");
		return false;
	}

	APlayerController* PC = OwnerAbility->GetPC();
	if (!PC)
	{
		pwlogability(Error, "ComputeTarget failed: PlayerController is null for %s", *OwnerAbility->GetName());
		return false;
	}
	
	// --- Initialize policies ---
	if (TargetingSource)
		TargetingSource->Initialize(OwnerAbility);

	if (TargetResolver)
		TargetResolver->Initialize(OwnerAbility);

	const UPWAbilityRangeModule* RangeModule = OwnerAbility->GetRangeModule();
	if (RangeModule && RangeModule->RangePolicy)
		RangeModule->RangePolicy->Initialize(OwnerAbility);
	
	// --- Targeting ---
	if (!TargetingSource || !TargetingSource->Sample(PC, Out))
	{
		return false;
	}

	// --- Range clamp ---
	if (RangeModule)
	{
		AActor* Pawn = OwnerAbility->GetPawn();
		if (Pawn)
		{
			RangeModule->ApplyRangePolicy(Pawn, Out);
		}
	}

	// --- Resolve targets ---
	TArray<AActor*> OutActors;
	if (TargetResolver)
	{
		if (const AActor* SourceActor = OwnerAbility->GetPawn())
		{
			TargetResolver->Resolve(SourceActor, Out, OutActors);
		}
	}


	// --- Copy valid actors ---
	Out.HitActors.Reset();
	for (AActor* A : OutActors)
	{
		if (IsValid(A))
			Out.HitActors.Add(A);
		else
			pwlogability(Verbose, "Skipping invalid actor in result");
	}

	// --- Finalize ---
	const FVector Origin = OwnerAbility->GetPawn()
		? OwnerAbility->GetPawn()->GetActorLocation()
		: FVector::ZeroVector;

	Out.Rotation = (Out.Location - Origin).Rotation();
	Out.bHasHit = true;

	pwlogability(Verbose, "ComputeTarget succeeded: %d targets found for %s", Out.HitActors.Num(), *OwnerAbility->GetName());
	return true;
}

void UPWAbilityTargetingModule::GetRequiredDataModules_Implementation(TArray<TSubclassOf<UPWDataModule>>& OutRequiredModules)
{
	if (TargetResolver)
		TargetResolver->GetRequiredDataModules(OutRequiredModules);
}
