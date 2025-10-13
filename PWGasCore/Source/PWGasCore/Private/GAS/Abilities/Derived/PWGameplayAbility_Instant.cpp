#include "GAS/Abilities/Derived/PWGameplayAbility_Instant.h"

void UPWGameplayAbility_Instant::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (GIsEditor)
	{
		UE_LOG(LogTemp, Verbose, TEXT("[%s] Instant Ability Activated."), *GetNameSafe(this));
	}
	
	if (bAutoCommit && !CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!bIsCancelable && !bIsActive)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
	}
}