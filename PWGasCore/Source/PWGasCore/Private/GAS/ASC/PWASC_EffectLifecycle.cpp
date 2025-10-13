// copyright mkurtt96


#include "GAS/ASC/PWASC_EffectLifecycle.h"

#include "GameplayEffect.h"
#include "GAS/ASC/PWAbilitySystemComponent.h"
#include "GAS/EffectContext/PWGameplayEffectContext.h"

void FPWASC_EffectLifecycle::ApplyBuffTag(const FGameplayTag BuffTag) const
{
	FGameplayEffectContextHandle EffectContext = ASC.MakeEffectContext();
	EffectContext.AddSourceObject(&ASC);

	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(*BuffTag.ToString()));
	Effect->DurationPolicy = EGameplayEffectDurationType::Infinite;
	Effect->InheritableOwnedTagsContainer.AddTag(BuffTag);

	if (const FGameplayEffectSpec* Spec = new FGameplayEffectSpec(Effect, EffectContext, 1))
	{
		FActiveGameplayEffectHandle ActiveEffectHandle = ASC.ApplyGameplayEffectSpecToSelf(*Spec);
	}
}

void FPWASC_EffectLifecycle::RemoveEffectsByTag(const FGameplayTagContainer& TagsToRemove) const
{
	FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(TagsToRemove);

	TArray<FActiveGameplayEffectHandle> HandlesToRemove = ASC.GetActiveEffects(Query);

	for (const FActiveGameplayEffectHandle& Handle : HandlesToRemove)
	{
		ASC.RemoveActiveGameplayEffect(Handle);
	}
}
