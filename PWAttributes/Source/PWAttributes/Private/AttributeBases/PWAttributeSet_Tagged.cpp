// copyright mkurtt96


#include "AttributeBases/PWAttributeSet_Tagged.h"

#include "GameplayTagContainer.h"

bool UPWAttributeSet_Tagged::GetAttributeForTag(const FGameplayTag& Tag, FGameplayAttribute& OutAttribute) const
{
	if (!Tag.IsValid()) return false;
	if (const FName* Name = TagToPropertyName.Find(Tag))
		if (FProperty* Prop = GetClass()->FindPropertyByName(*Name))
		{
			FGameplayAttribute GA;
			GA.SetUProperty(Prop);
			OutAttribute = GA;
			return true;
		}
	return false;
}

FGameplayAttribute UPWAttributeSet_Tagged::MakeAttribute(const UClass* SetClass, const FName PropertyName)
{
	if (!SetClass || PropertyName.IsNone()) return FGameplayAttribute();
	if (FProperty* Prop = SetClass->FindPropertyByName(PropertyName))
	{
		FGameplayAttribute GA;
		GA.SetUProperty(Prop);
		return GA;
	}
	return FGameplayAttribute();
}
