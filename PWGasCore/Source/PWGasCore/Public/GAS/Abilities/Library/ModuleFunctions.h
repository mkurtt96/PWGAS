﻿// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ModuleFunctions.generated.h"

class UEffectSpellParamsExtension;
class USpellParams;
/**
 * 
 */
UCLASS()
class PWGASCORE_API UModuleFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="GASCore|SpellParams")
	static UEffectSpellParamsExtension* GetEffectParams(const USpellParams* Params);
	UFUNCTION(BlueprintPure, BlueprintPure, Category="Accessors")
	static float GetRange(const USpellParams* Params);
	UFUNCTION(BlueprintPure, BlueprintPure, Category="Accessors")
	static float GetRadius(const USpellParams* Params);
};
