// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "PWAbilityModule.h"
#include "Ability/SpellParams.h"
#include "PWAbilityActorModule.generated.h"

struct FPWTargetingResult;

UCLASS(BlueprintType)
class PWGASCORE_API UActorSpellParams : public USpellParamsBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector SpawnLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRotator SpawnRotation = FRotator::ZeroRotator;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Duration = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bAttachToOwner = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName AttachSocket = NAME_None;
};


UCLASS(Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class PWGASCORE_API UPWAbilityActorModule : public UPWAbilityModule
{
	GENERATED_BODY()

public:
	/** Actor class to spawn. Must implement PWSpellParamsInterface. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Actor|Spawn", meta=(MustImplement="PWSpellParamsInterface"))
	TSubclassOf<AActor> ActorClass;

	/** If true, the spawned actor will be attached to the ability owner. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Actor|Spawn")
	bool bAttachToOwner = false;

	/** Socket name to attach to when bAttachToOwner = true. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Actor|Spawn")
	FName SpawnSocket = NAME_None;
	
	/** Optional local offset applied after forward offset. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Actor|Spawn")
	FVector SpawnRelativeOffset = FVector::ZeroVector;

	/** If true, pitch will be flattened (useful for ground spawns). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Actor|Spawn")
	bool bZeroPitch = false;

	/** Optional duration for the spawned actor (passed through SpellParams). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Actor|Lifetime")
	FScalableFloat Duration = 0.f;

public:
	UFUNCTION(BlueprintCallable, Category="Actor")
	virtual USpellParamsBase* MakeSpellParams(const FPWTargetingResult& Target) const;

	UFUNCTION(BlueprintCallable, Category="Actor")
	virtual AActor* SpawnActor(const FPWTargetingResult& Target) const;

protected:
	virtual FTransform BuildSpawnTransform(const FPWTargetingResult& Target) const;
};