// // copyright mkurtt96
//
//
// #include "AttributeSetPerAttribute/BasicAttributes/AS_MovementSpeed.h"
//
// DefineAttributeGameplayTags_Basic(MovementSpeed);
//
// void UAS_MovementSpeed::PreAttributeChange(const FGameplayAttribute& Attr, float& NewValue)
// {
// 	Super::PreAttributeChange(Attr, NewValue);
// 	if (Attr == GetValueAttribute())
// 	{
// 		NewValue = FMath::Max(0.f, NewValue);
// 	}
// }
