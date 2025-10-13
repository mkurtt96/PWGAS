// copyright mkurtt96


#include "PWGasCore/Public/GAS/Abilities/Modules/PWAbilityModule.h"

#include "GAS/Abilities/PWGameplayAbilityBase.h"

void UPWAbilityModule::Initialize(UPWGameplayAbilityBase* InOwner)
{
	OwnerAbility = InOwner;
	
#if WITH_EDITOR
	if (OwnerAbility)
	{
		int32 Seen = 0;
		for (UPWAbilityModule* M : OwnerAbility->AbilityModules)
		{
			if (!M) continue;
			if (M->GetClass() == GetClass())
			{
				if (M == this) { ModuleIndex = Seen; break; }
				++Seen;
			}
		}
	}
#endif
}

#if WITH_EDITOR
void UPWAbilityModule::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

APlayerController* UPWAbilityModule::GetPC() const
{
	return OwnerAbility ? OwnerAbility->GetPC() : nullptr;
}

APawn* UPWAbilityModule::GetPawn() const
{
	return OwnerAbility ? OwnerAbility->GetPawn() : nullptr;
}