// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "PWAbilityActorModule.h"
#include "PWActionModule.h"
#include "Targeting/Data/PWTargetingData.h"
#include "PWAbilityProjectileModule.generated.h"

UCLASS(BlueprintType)
class PWGASCORE_API UProjectileSpellParamsExtension : public UActorSpellParamsExtension
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float InitialSpeed = 1200.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float GravityZOverride = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ProjectileRadius = 8.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Range = 0.f;
};

/**
 * 
 */
UCLASS()
class PWGASCORE_API UPWAbilityProjectileModule : public UPWAbilityActorModule
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FPWProjectileSimConfig ProjectileConfig;
	
	UFUNCTION(BlueprintCallable, Category="Module")
	virtual AActor* SpawnProjectile(const FPWTargetingResult& Target);

	UFUNCTION(BlueprintPure, Category="Module")
	const FPWProjectileSimConfig& GetProjectileConfig() const;

protected:
	virtual USpellParams* MakeSpellParams_Implementation(const FPWTargetingResult& Target) override;
};
