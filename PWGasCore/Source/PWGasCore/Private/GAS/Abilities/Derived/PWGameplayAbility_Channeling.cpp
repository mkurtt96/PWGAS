﻿// copyright mkurtt96


#include "GAS/Abilities/Derived/PWGameplayAbility_Channeling.h"

void UPWGameplayAbility_Channeling::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (bAutoStartChannel)
		StartChanneling();
}

void UPWGameplayAbility_Channeling::StartChanneling()
{
	if (bIsChanneling || !GetWorld()) return;

	bIsChanneling = true;
	CurrentChannelTime = 0.f;

	OnChannelStart();

	GetWorld()->GetTimerManager().SetTimer(
		ChannelTickTimer,
		this,
		&UPWGameplayAbility_Channeling::HandleChannelTick,
		ChannelTickInterval,
		true
	);
}

void UPWGameplayAbility_Channeling::HandleChannelTick()
{
	if (!bIsChanneling) return;

	CurrentChannelTime += ChannelTickInterval;
	OnChannelTick(CurrentChannelTime);

	if (MaxChannelDuration > 0.f && CurrentChannelTime >= MaxChannelDuration)
	{
		StopChanneling(false);
	}
}

void UPWGameplayAbility_Channeling::PauseChanneling()
{
	if (!bIsChanneling) return;
	GetWorld()->GetTimerManager().PauseTimer(ChannelTickTimer);
}

void UPWGameplayAbility_Channeling::StopChanneling(const bool bWasCanceled)
{
	if (!bIsChanneling) return;

	bIsChanneling = false;
	GetWorld()->GetTimerManager().ClearTimer(ChannelTickTimer);

	OnChannelEnd(bWasCanceled);

	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, bWasCanceled);
}

float UPWGameplayAbility_Channeling::GetChannelPercentage() const
{
	if (MaxChannelDuration <= 0.f)
		return 1.f;

	return FMath::Clamp(CurrentChannelTime / MaxChannelDuration, 0.f, 1.f);
}
