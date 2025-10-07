// copyright mkurtt96


#include "BasicAttributes/AS_MovementSpeed.h"

DefineGameplayTags_Basic(MovementSpeed);

void UAS_MovementSpeed::PreAttributeChange(const FGameplayAttribute& Attr, float& NewValue)
{
	Super::PreAttributeChange(Attr, NewValue);
	if (Attr == GetValueAttribute())
	{
		NewValue = FMath::Max(0.f, NewValue);
	}
}
