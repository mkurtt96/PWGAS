// copyright mkurtt96


#include "AttributeSetPerAttribute/Library/PWAttributeFunctions.h"

#include "AbilitySystemComponent.h"
#include "AttributeSetPerAttribute/PWAttributeSet_Tagged.h"


bool UPWAttributeFunctions::ResolveAttributeOnASC(const UAbilitySystemComponent* ASC, const FGameplayTag& Tag, FGameplayAttribute& Out)
{
	if (!ASC || !Tag.IsValid()) return false;

	// Iterate all spawned sets; the first tagged set that resolves wins.
	for (UAttributeSet* Set : ASC->GetSpawnedAttributes())
	{
		if (const UPWAttributeSet_Tagged* T = Cast<UPWAttributeSet_Tagged>(Set))
		{
			if (T->GetAttributeForTag(Tag, Out))
			{
				return true;
			}
		}
	}
	return false;
}

FGameplayAttribute UPWAttributeFunctions::GetAttribute(const UAbilitySystemComponent* ASC, const FGameplayTag Tag)
{
	FGameplayAttribute Attr;
	ResolveAttributeOnASC(ASC, Tag, Attr);
	return Attr;
}

float UPWAttributeFunctions::GetAttributeValue(const UAbilitySystemComponent* ASC, const FGameplayTag Tag)
{
	FGameplayAttribute Attr;
	if (ResolveAttributeOnASC(ASC, Tag, Attr))
	{
		return ASC->GetNumericAttribute(Attr);
	}
	return 0.f;
}

void UPWAttributeFunctions::SetAttributeValue(UAbilitySystemComponent* ASC, const FGameplayTag Tag, const float NewBase)
{
	if (!ASC) return;

	FGameplayAttribute Attr;
	if (ResolveAttributeOnASC(ASC, Tag, Attr))
	{
		ASC->SetNumericAttributeBase(Attr, NewBase);
	}
}
