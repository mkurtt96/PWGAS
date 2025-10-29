﻿// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/Modules/PWAbilityModule.h"
#include "PWActionModule.generated.h"

class UPWDataModule;
class USpellParams;
struct FPWTargetingResult;
/**
 * 
 */
UCLASS()
class PWGASCORE_API UPWActionModule : public UPWAbilityModule
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category="Module")
	USpellParams* MakeSpellParams(const FPWTargetingResult& Target);
	virtual USpellParams* MakeSpellParams_Implementation(const FPWTargetingResult& Target);

	virtual void ExtendParamsWithData(USpellParams* Params) const;
};
