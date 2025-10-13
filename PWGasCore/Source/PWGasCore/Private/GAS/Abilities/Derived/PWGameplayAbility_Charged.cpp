// copyright mkurtt96


#include "GAS/Abilities/Derived/PWGameplayAbility_Charged.h"

void UPWGameplayAbility_Charged::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (bAutoStartCharge)
		StartCharging();
}

void UPWGameplayAbility_Charged::StartCharging()
{
	if (bIsCharging || !GetWorld()) return;

	bIsCharging = true;
	CurrentChargeTime = 0.f;

	OnChargeStart();

	GetWorld()->GetTimerManager().SetTimer(
		ChargeTickTimer,
		this,
		&UPWGameplayAbility_Charged::HandleChargeTick,
		ChargeTickInterval,
		true
	);
}

void UPWGameplayAbility_Charged::PauseCharging()
{
	if (!bIsCharging) return;
	GetWorld()->GetTimerManager().PauseTimer(ChargeTickTimer);
}

void UPWGameplayAbility_Charged::StopCharging(bool bWasCanceled)
{
	if (!bIsCharging) return;
	bIsCharging = false;

	GetWorld()->GetTimerManager().ClearTimer(ChargeTickTimer);

	if (bWasCanceled)
		OnChargeCanceled();

	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, bWasCanceled);
}

float UPWGameplayAbility_Charged::GetChargePercentage() const
{
	if (ChargeDuration <= 0.f)
		return 1.f;
	
	return FMath::Clamp(CurrentChargeTime / ChargeDuration, 0.f, 1.f);
}

void UPWGameplayAbility_Charged::HandleChargeTick()
{
	if (!bIsCharging) return;

	CurrentChargeTime += ChargeTickInterval;
	const float Normalized = FMath::Clamp(CurrentChargeTime / ChargeDuration, 0.f, 1.f);

	OnChargeTick(Normalized);

	if (CurrentChargeTime >= ChargeDuration)
	{
		StopCharging(false);
		OnChargeComplete();
	}
}
