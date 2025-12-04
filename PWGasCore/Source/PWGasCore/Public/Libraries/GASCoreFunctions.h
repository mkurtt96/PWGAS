// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GASCoreFunctions.generated.h"

enum class EGameplayEffectDurationType : uint8;
struct FGameplayAttribute;
class UPWAbilitySystemComponent;
class USpellParams;

USTRUCT(BlueprintType)
struct FEffectModifierValue
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	EGameplayEffectDurationType DurationType;
	UPROPERTY(BlueprintReadWrite)
	float Duration;
	UPROPERTY(BlueprintReadWrite)
	float Period;
	UPROPERTY(BlueprintReadWrite)
	float Value;
};

UCLASS()
class PWGASCORE_API UGASCoreFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static bool ApplyEffects(UPWAbilitySystemComponent* ASC, const USpellParams* SpellParams);
	static TArray<FEffectModifierValue> GetModifierValuesFromEffects(const USpellParams* SpellParams, const FGameplayAttribute& IncomingDamageAttr);
};
