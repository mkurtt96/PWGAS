// // copyright mkurtt96
//
// #include "PWAttributes/Public/AttributeSetPerAttribute/BasicAttributes/AS_Health.h"
//
// #include "GameplayEffectExtension.h"
//
// DefineAttributeGameplayTags_Regenerating(Health);
//
// void UAS_Health::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
// {
// 	Super::PostGameplayEffectExecute(Data);
//
// 	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
// 	{
// 		const float Dmg = FMath::Max(0.f, GetIncomingDamage());
// 		SetIncomingDamage(0.f);
//
// 		const float NewHP = FMath::Clamp(GetValue() - Dmg, 0.f, GetMax());
// 		SetValue(NewHP);
// 	}
// }
