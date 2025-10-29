// copyright mkurtt96


#include "PWGasCore/Public/GAS/Abilities/Modules/PWAbilityModule.h"

#include "GAS/Abilities/PWGameplayAbilityBase.h"

void UPWAbilityModule::Initialize(UPWGameplayAbilityBase* InOwner)
{
	OwnerAbility = InOwner;
}

void UPWAbilityModule::OnAbilityEnded_Implementation()
{
}

void UPWAbilityModule::OnAbilityActivated_Implementation()
{
}

void UPWAbilityModule::GetRequiredDataModules_Implementation(TArray<TSubclassOf<UPWDataModule>>& OutRequiredModules)
{
}

APlayerController* UPWAbilityModule::GetPC() const
{
	return OwnerAbility ? OwnerAbility->GetPC() : nullptr;
}

APawn* UPWAbilityModule::GetPawn() const
{
	return OwnerAbility ? OwnerAbility->GetPawn() : nullptr;
}
