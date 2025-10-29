// copyright mkurtt96


#include "PWGasCore/Public/GAS/Abilities/Modules/DataModules/PWAbilityEffectModule.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GAS/Abilities/PWGameplayAbilityBase.h"

void UEffectSpellParamsExtension::ApplyEffectsToTarget(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, const int32 AbilityLevel) const
{
	if (!SourceASC || !TargetASC)
		return;

	for (const FEffectEntry& Entry : Effects)
	{
		if (!Entry.EffectClass)
			continue;

		FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
		Context.AddSourceObject(SourceASC->GetAvatarActor());

		FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(Entry.EffectClass, AbilityLevel, Context);
		if (!SpecHandle.IsValid())
			continue;

		if (FGameplayEffectSpec* Spec = SpecHandle.Data.Get())
		{
			if (Entry.StackCount > 1)
				Spec->SetStackCount(Entry.StackCount);
		}

		SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
	}
}

void UPWAbilityEffectModule::ExtendParams(USpellParams* Params) const
{
	if (!Params)
		return;

	UEffectSpellParamsExtension* Extension = Params->AddExtension<UEffectSpellParamsExtension>();
	Extension->Effects = Effects;
	Params->TargetEffectMask = TargetMask;
}

void UPWAbilityEffectModule::ApplyEffects(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
{
	if (!SourceASC || !TargetASC)
		return;

	for (const FEffectEntry& Entry : Effects)
	{
		if (!Entry.EffectClass)
			continue;

		FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
		Context.AddSourceObject(SourceASC->GetAvatarActor());

		FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(Entry.EffectClass, OwnerAbility->GetAbilityLevel(), Context);
		if (!SpecHandle.IsValid())
			continue;

		if (FGameplayEffectSpec* Spec = SpecHandle.Data.Get())
		{
			if (Entry.StackCount > 1)
				Spec->SetStackCount(Entry.StackCount);
		}

		FActiveGameplayEffectHandle Handle = SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);

		if (Handle.IsValid())
			ActiveEffectHandles.FindOrAdd(TargetASC).Add(Handle);
	}
}

void UPWAbilityEffectModule::RemoveEffects(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
{
	if (!SourceASC || !TargetASC)
		return;

	if (TArray<FActiveGameplayEffectHandle>* Handles = ActiveEffectHandles.Find(TargetASC))
	{
		for (const FActiveGameplayEffectHandle& Handle : *Handles)
		{
			TargetASC->RemoveActiveGameplayEffect(Handle);
		}
		ActiveEffectHandles.Remove(TargetASC);
	}
}
