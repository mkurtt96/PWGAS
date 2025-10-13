// copyright mkurtt96


#include "GAS/Abilities/Modules/PWAbilityTargetingModule.h"

#include "GAS/Abilities/PWGameplayAbilityBase.h"
#include "GAS/Abilities/Modules/PWAbilityRangeModule.h"
#include "Targeting/Interfaces/PWTargetingInterfaces.h"

bool UPWAbilityTargetingModule::ComputeTarget(FPWTargetingResult& Out)
{
	if (!OwnerAbility) return false;
	APlayerController* PC = OwnerAbility->GetPC();
	if (!PC) return false;

	UObject* TargetingObj = nullptr;
	if (TargetingSourceClass)
		TargetingObj = NewObject<UObject>(this, TargetingSourceClass);

	if (IPWTargetingSource* T = Cast<IPWTargetingSource>(TargetingObj))
	{
		if (!T->Execute_Sample(TargetingObj, PC, Out))
			return false;
	}
	else
		return false;

	if (bClampToRange)
		if (const UPWAbilityRangeModule* RangeModule = OwnerAbility->GetModule<UPWAbilityRangeModule>())
			RangeModule->ApplyRangePolicy(OwnerAbility->GetPawn(), Out);

	const FVector Origin = OwnerAbility->GetPawn() ? OwnerAbility->GetPawn()->GetActorLocation() : FVector::ZeroVector;
	Out.Rotation = (Out.Location - Origin).Rotation();
	return true;
}
