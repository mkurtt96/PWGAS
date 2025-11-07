// copyright mkurtt96


#include "GAS/ASC/PWAbilitySystemComponent.h"

#include "GAS/Abilities/PWGameplayAbilityBase.h"
#include "GAS/ASC/PWASC_AbilityLifecycle.h"
#include "GAS/ASC/PWASC_DataManagement.h"
#include "GAS/ASC/PWASC_EffectLifecycle.h"
#include "GAS/ASC/PWASC_InputBinding.h"
#include "GAS/Data/GASCoreEnums.h"
#include "GAS/Tags/GASCoreTags.h"

UPWAbilitySystemComponent::UPWAbilitySystemComponent()
{
	DataManagement = nullptr;
	AbilityLifecycle = nullptr;
	EffectLifecycle = nullptr;
	InputBinding = nullptr;
	InputBinding = nullptr;
}

void UPWAbilitySystemComponent::OnRegister()
{
	Super::OnRegister();

	if (!DataManagement) DataManagement = CreateDataManagement();
	if (!AbilityLifecycle) AbilityLifecycle = CreateAbilityLifecycle();
	if (!EffectLifecycle) EffectLifecycle = CreateEffectLifecycle();
	if (!InputBinding) InputBinding = CreateInputBinding();

	GameplayTagCountContainer.RegisterGenericGameplayEvent().AddLambda([this](FGameplayTag Tag, int32 Count)
	{
		AnyGameplayTagChanged.Broadcast(Tag, Count);
	});
}

void UPWAbilitySystemComponent::OnUnregister()
{
	Super::OnUnregister();
	InputBinding.Reset();
	EffectLifecycle.Reset();
	AbilityLifecycle.Reset();
	DataManagement.Reset();
}

void UPWAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPWAbilitySystemComponent::OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnRemoveAbility(AbilitySpec);
}

void UPWAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	AbilityLifecycle->AbilitiesUpdated.Broadcast();
}

void UPWAbilitySystemComponent::BP_AddAbilities(const FGameplayTagContainer& AbilityTags)
{
	AbilityLifecycle->AddAbilities(AbilityTags);
}

void UPWAbilitySystemComponent::BP_AddAbility(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag)
{
	AbilityLifecycle->AddAbility(AbilityTag, InputTag);
}

void UPWAbilitySystemComponent::BP_RemoveAbility(const FGameplayTag& AbilityTag)
{
	AbilityLifecycle->RemoveAbility(AbilityTag);
}

void UPWAbilitySystemComponent::BP_UpgradeAbility(const FGameplayTag& AbilityTag)
{
	AbilityLifecycle->UpgradeAbility(AbilityTag);
}

void UPWAbilitySystemComponent::BP_DowngradeAbility(const FGameplayTag& AbilityTag)
{
	AbilityLifecycle->DowngradeAbility(AbilityTag);
}

bool UPWAbilitySystemComponent::BP_GetCooldownRemainingForTag(const FGameplayTag& CooldownTag, float& TimeRemaining, float& CooldownDuration) const
{
	return CooldownHandler->GetCooldownRemainingForTag(CooldownTag, TimeRemaining, CooldownDuration);
}

bool UPWAbilitySystemComponent::BP_GetCooldownRemainingForAbilityTag(const FGameplayTag& AbilityTag, float& TimeRemaining, float& TotalDuration) const
{
	return CooldownHandler->GetCooldownRemainingForAbilityTag(AbilityTag, TimeRemaining, TotalDuration);
}

int32 UPWAbilitySystemComponent::BP_ClearCooldownByTag(const FGameplayTag& CooldownTag)
{
	return CooldownHandler->ClearCooldownByTag(CooldownTag);
}

int32 UPWAbilitySystemComponent::BP_ClearCooldownForAbilityTag(const FGameplayTag& AbilityTag)
{
	return CooldownHandler->ClearCooldownForAbilityTag(AbilityTag);
}

bool UPWAbilitySystemComponent::BP_AdjustCooldownForTag(const FGameplayTag& CooldownTag, float SecondsToReduce)
{
	return CooldownHandler->AdjustCooldownForTag(CooldownTag, SecondsToReduce);
}

bool UPWAbilitySystemComponent::BP_AdjustCooldownForAbilityTag(const FGameplayTag& AbilityTag, float SecondsToReduce)
{
	return CooldownHandler->AdjustCooldownForAbilityTag(AbilityTag, SecondsToReduce);
}

void UPWAbilitySystemComponent::ServerSendInputEvent_Implementation(const FGameplayTag& EventTag, const FGameplayEventData& Payload)
{
	HandleGameplayEvent(EventTag, &Payload);
}

void UPWAbilitySystemComponent::SendInputEvent(const FGameplayTag& InputTag, EInputEventType EventType)
{
	if (!InputTag.IsValid()) return;

	FGameplayEventData Payload;
	Payload.EventTag = PWTags::Event::Input;
	Payload.EventMagnitude = (EventType == EInputEventType::Pressed) ? 1.f : 0.f; // 1=Pressed, 0=Released
	Payload.InstigatorTags.AddTag(InputTag); // carry which input

	// Local (prediction): let locally-running abilities react immediately.
	HandleGameplayEvent(PWTags::Event::Input, &Payload);

	// If we're a client, mirror to server for authoritative handling.
	if (!IsOwnerActorAuthoritative())
	{
		ServerSendInputEvent(PWTags::Event::Input, Payload);
	}
}

TUniquePtr<FPWASC_DataManagement> UPWAbilitySystemComponent::CreateDataManagement()
{
	return MakeUnique<FPWASC_DataManagement>(*this);
}

TUniquePtr<FPWASC_AbilityLifecycle> UPWAbilitySystemComponent::CreateAbilityLifecycle()
{
	return MakeUnique<FPWASC_AbilityLifecycle>(*this);
}

TUniquePtr<FPWASC_EffectLifecycle> UPWAbilitySystemComponent::CreateEffectLifecycle()
{
	return MakeUnique<FPWASC_EffectLifecycle>(*this);
}

TUniquePtr<FPWASC_InputBinding> UPWAbilitySystemComponent::CreateInputBinding()
{
	return MakeUnique<FPWASC_InputBinding>(*this);
}

TUniquePtr<UPWASC_CooldownHandler> UPWAbilitySystemComponent::CreateCooldownHandler()
{
	return MakeUnique<UPWASC_CooldownHandler>(*this);
}
