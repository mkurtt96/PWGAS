// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "PWAbilityActorModule.h"
#include "Ability/SpellParams.h"
#include "Targeting/Data/PWTargetingTypes.h"
#include "PWAbilityProjectileModule.generated.h"

UCLASS()
class PWGASCORE_API UProjectileSpellParams : public USpellParamsBase
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

UCLASS(Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class PWGASCORE_API UPWAbilityProjectileModule : public UPWAbilityActorModule
{
	GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile|Config")
    FPWProjectileSimConfig ProjectileConfig;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Targeting|CollisionFilters", meta=(Bitmask, BitmaskEnum="/Script/PWSharedTypes.ERelationMask"))
    int32 TargetCollisionMask = (int32)ERelationMask::All;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Targeting|CollisionFilters", meta=(Bitmask, BitmaskEnum="/Script/PWSharedTypes.ERelationMask"))
    int32 TargetEffectMask = (int32)ERelationMask::All;

    
    virtual USpellParamsBase* MakeSpellParams(const FPWTargetingResult& Target) const override;
    
    UFUNCTION(BlueprintPure, Category="Projectile")
    const FPWProjectileSimConfig& GetProjectileConfig() const { return ProjectileConfig; }

protected:
};