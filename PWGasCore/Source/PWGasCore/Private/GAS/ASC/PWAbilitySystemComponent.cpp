// copyright mkurtt96


#include "GAS/ASC/PWAbilitySystemComponent.h"

#include "GAS/Abilities/PWGameplayAbilityBase.h"
#include "GAS/ASC/PWASC_AbilityLifecycle.h"
#include "GAS/ASC/PWASC_DataManagement.h"
#include "GAS/ASC/PWASC_EffectLifecycle.h"
#include "GAS/ASC/PWASC_InputBinding.h"
#include "GAS/Tags/GASCoreTags.h"

UPWAbilitySystemComponent::UPWAbilitySystemComponent()
{
	DataManagement = nullptr;
	AbilityLifecycle = nullptr;
	EffectLifecycle = nullptr;
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
	if (InputBinding->FocusedAbility && InputBinding->FocusedAbility->Handle == AbilitySpec.Handle)
		InputBinding->FocusedAbility = nullptr;
}

void UPWAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	AbilityLifecycle->AbilitiesUpdated.Broadcast();
}

bool UPWAbilitySystemComponent::GetCooldownRemainingForTag(const FGameplayTag& CooldownTag, float& TimeRemaining, float& CooldownDuration) const
{
	FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
	TArray<TPair<float, float>> DurationAndTimeRemaining = GetActiveEffectsTimeRemainingAndDuration(Query);
	if (DurationAndTimeRemaining.Num() > 0)
	{
		int32 BestIdx = 0;
		float LongestTime = DurationAndTimeRemaining[0].Key;
		for (int32 Idx = 1; Idx < DurationAndTimeRemaining.Num(); ++Idx)
		{
			if (DurationAndTimeRemaining[Idx].Key > LongestTime)
			{
				LongestTime = DurationAndTimeRemaining[Idx].Key;
				BestIdx = Idx;
			}
		}

		TimeRemaining = DurationAndTimeRemaining[BestIdx].Key;
		CooldownDuration = DurationAndTimeRemaining[BestIdx].Value;
		return true;
	}
	return false;
}

bool UPWAbilitySystemComponent::GetCooldownRemainingForAbilityTag(const FGameplayTag& AbilityTag, float& TimeRemaining, float& TotalDuration) const
{
	const FGameplayTag CooldownTag = PWTags::Ability::Skill::GetCooldownTag(AbilityTag);
	return CooldownTag.IsValid() ? GetCooldownRemainingForTag(CooldownTag, TimeRemaining, TotalDuration) : 0;
}

int32 UPWAbilitySystemComponent::ClearCooldownByTag(const FGameplayTag& CooldownTag)
{
	FGameplayTagContainer Tags;
	Tags.AddTag(CooldownTag);
	return RemoveActiveEffectsWithGrantedTags(Tags);
}

int32 UPWAbilitySystemComponent::ClearCooldownForAbilityTag(const FGameplayTag& AbilityTag)
{
	const FGameplayTag CooldownTag = PWTags::Ability::Skill::GetCooldownTag(AbilityTag);
	return CooldownTag.IsValid() ? ClearCooldownByTag(CooldownTag) : 0;
}

bool UPWAbilitySystemComponent::AdjustCooldownForTag(const FGameplayTag& CooldownTag, float SecondsToReduce)
{
	float Remaining = 0, Total = 0;
	if (GetCooldownRemainingForTag(CooldownTag, Remaining, Total) || Remaining <= 0.f) return false;

	const FActiveGameplayEffect* ConstEffect = Data().GetSpecOfEffect(CooldownTag);
	if (!ConstEffect)
		return false;

	FActiveGameplayEffect* Effect = const_cast<FActiveGameplayEffect*>(ConstEffect);
	const float Duration = Remaining + SecondsToReduce;
	Effect->Spec.bDurationLocked = false;
	Effect->Spec.SetDuration(Duration, true);
	return true;
}

bool UPWAbilitySystemComponent::AdjustCooldownForAbilityTag(const FGameplayTag& AbilityTag, float SecondsToReduce)
{
	const FGameplayTag CooldownTag = PWTags::Ability::Skill::GetCooldownTag(AbilityTag);
	return CooldownTag.IsValid() ? AdjustCooldownForTag(CooldownTag, SecondsToReduce) : 0;
}

void UPWAbilitySystemComponent::SendTaggedAbilityInput(FGameplayAbilitySpec AbilitySpec, EPWInputEventType InputType, FGameplayTag InputTag)
{
	if (!InputTag.IsValid() || !IsValid(AbilitySpec.Ability))
		return;

	const FPredictionKey PredictionKey = GetPredictionKeyForNewAction();

	FScopedPredictionWindow ScopedPrediction(this, PredictionKey);

		if (AbilitySpec.IsActive() && AbilitySpec.Ability &&
			AbilitySpec.Ability->GetClass()->ImplementsInterface(UPWTaggedAbilityInput::StaticClass()))
		{
			IPWTaggedAbilityInput::Execute_HandleTaggedAbilityInput(AbilitySpec.Ability, InputType, InputTag);
			if (!IsOwnerActorAuthoritative())
			{
				ServerSendTaggedAbilityInput(AbilitySpec.Handle, PredictionKey, InputType, InputTag);
			}
		}
	
}

void UPWAbilitySystemComponent::ServerSendTaggedAbilityInput_Implementation(FGameplayAbilitySpecHandle AbilityHandle, FPredictionKey PredictionKey, EPWInputEventType InputType, FGameplayTag InputTag)
{
	if (!AbilityHandle.IsValid() || !InputTag.IsValid())
		return;

	FScopedPredictionWindow Scoped(this, PredictionKey);

	if (FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(AbilityHandle))
	{
		if (Spec->IsActive() && Spec->Ability &&
			Spec->Ability->GetClass()->ImplementsInterface(UPWTaggedAbilityInput::StaticClass()))
		{
			IPWTaggedAbilityInput::Execute_HandleTaggedAbilityInput(Spec->Ability, InputType, InputTag);
		}
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
