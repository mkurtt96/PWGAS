#pragma once

#include "PWPrecastTypes.generated.h"

USTRUCT(BlueprintType)
struct FPWPrecastVisualUpdate
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite) TArray<FVector> PathPoints;
	UPROPERTY(BlueprintReadWrite) FVector ImpactPoint = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite) FVector ImpactNormal = FVector::UpVector;
	UPROPERTY(BlueprintReadWrite) float Radius = 0.f;
	UPROPERTY(BlueprintReadWrite) bool bValid = true;
};
