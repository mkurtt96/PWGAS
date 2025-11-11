// // copyright mkurtt96
//
//
// #include "AttributeSetPerAttribute/PWAttributeSet_RegeneratingAttribute.h"
// #include "Net/UnrealNetwork.h"
// #include "GameplayEffectExtension.h"
//
// void UPWAttributeSet_RegeneratingAttribute::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
// {
// 	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
// 	
// 	DOREPLIFETIME_CONDITION_NOTIFY(UPWAttributeSet_RegeneratingAttribute, Value, COND_None, REPNOTIFY_Always);
// 	DOREPLIFETIME_CONDITION_NOTIFY(UPWAttributeSet_RegeneratingAttribute, Max,     COND_None, REPNOTIFY_Always);
// 	DOREPLIFETIME_CONDITION_NOTIFY(UPWAttributeSet_RegeneratingAttribute, Regen,     COND_None, REPNOTIFY_Always);
// }
//
// void UPWAttributeSet_RegeneratingAttribute::OnRep_Value(const FGameplayAttributeData& OldValue) const
// {
// 	GAMEPLAYATTRIBUTE_REPNOTIFY(UPWAttributeSet_RegeneratingAttribute, Value, OldValue);
// }
//
// void UPWAttributeSet_RegeneratingAttribute::OnRep_Max(const FGameplayAttributeData& OldValue) const
// {
// 	GAMEPLAYATTRIBUTE_REPNOTIFY(UPWAttributeSet_RegeneratingAttribute, Max, OldValue);
// }
//
// void UPWAttributeSet_RegeneratingAttribute::OnRep_Regen(const FGameplayAttributeData& OldValue) const
// {
// 	GAMEPLAYATTRIBUTE_REPNOTIFY(UPWAttributeSet_RegeneratingAttribute, Regen, OldValue);
// }
//
// void UPWAttributeSet_RegeneratingAttribute::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
// {
// 	Super::PreAttributeChange(Attribute, NewValue);
// 	
// 	if (Attribute == GetMaxAttribute())
// 	{
// 		NewValue = FMath::Max(0.f, NewValue);
// 		const float NewCurrent = FMath::Clamp(GetValue(), 0.f, NewValue);
// 		if (UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent())
// 		{
// 			ASC->SetNumericAttributeBase(GetValueAttribute(), NewCurrent);
// 		}
// 	}
// 	else if (Attribute == GetValueAttribute())
// 	{
// 		NewValue = FMath::Clamp(NewValue, 0.f, GetMax());
// 	}
// }
//
// void UPWAttributeSet_RegeneratingAttribute::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
// {
// 	Super::PostGameplayEffectExecute(Data);
// 	
// 	if (Data.EvaluatedData.Attribute == GetValueAttribute())
// 	{
// 		SetValue(FMath::Clamp(GetValue(), 0.f, GetMax()));
// 	}
// }
