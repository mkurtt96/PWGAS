// // copyright mkurtt96
//
//
// #include "AttributeSetPerAttribute/PWAttributeSet_BasicAttribute.h"
// #include "Net/UnrealNetwork.h"
//
// void UPWAttributeSet_BasicAttribute::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
// {
// 	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
// 	
// 	DOREPLIFETIME_CONDITION_NOTIFY(UPWAttributeSet_BasicAttribute, Value, COND_None, REPNOTIFY_Always);
// }
//
// void UPWAttributeSet_BasicAttribute::OnRep_Value(const FGameplayAttributeData& OldValue) const
// {
// 	GAMEPLAYATTRIBUTE_REPNOTIFY(UPWAttributeSet_BasicAttribute, Value, OldValue);
// }