#pragma once

#include "PWTargetingData.generated.h"

USTRUCT(BlueprintType)
struct FPWTargetingResult
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly) FVector Location = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly) FRotator Rotation = FRotator::ZeroRotator;
	UPROPERTY(BlueprintReadOnly) bool bHasHit = false;
	UPROPERTY(BlueprintReadOnly) FHitResult Hit;
	UPROPERTY(BlueprintReadOnly) TArray<AActor*> HitActors;

	AActor* GetPrimaryActor() const
	{
		return (HitActors.Num() > 0 && HitActors[0]) ? HitActors[0] : Hit.GetActor();
	}
};

USTRUCT(BlueprintType)
struct FPWProjectileSimConfig
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float InitialSpeed = 1200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float GravityZOverride = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float ProjectileRadius = 8.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float MaxSimTime = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float SimFrequency = 30.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bAlignToGroundNormal = true;
};