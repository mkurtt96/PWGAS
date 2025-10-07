// copyright mkurtt96

#include "AttributeBases/PWAttributeSet_ClampedAttribute.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"

void UPWAttributeSet_ClampedAttribute::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UPWAttributeSet_ClampedAttribute, Value, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPWAttributeSet_ClampedAttribute, Max,     COND_None, REPNOTIFY_Always);
}

void UPWAttributeSet_ClampedAttribute::OnRep_Value(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPWAttributeSet_ClampedAttribute, Value, OldValue);
}

void UPWAttributeSet_ClampedAttribute::OnRep_Max(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPWAttributeSet_ClampedAttribute, Max, OldValue);
}

void UPWAttributeSet_ClampedAttribute::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	if (Attribute == GetMaxAttribute())
	{
		NewValue = FMath::Max(0.f, NewValue);
		const float NewCurrent = FMath::Clamp(GetValue(), 0.f, NewValue);
		if (UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent())
		{
			ASC->SetNumericAttributeBase(GetValueAttribute(), NewCurrent);
		}
	}
	else if (Attribute == GetValueAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMax());
	}
}

void UPWAttributeSet_ClampedAttribute::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	if (Data.EvaluatedData.Attribute == GetValueAttribute())
	{
		SetValue(FMath::Clamp(GetValue(), 0.f, GetMax()));
	}
}