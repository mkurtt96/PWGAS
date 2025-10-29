// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "PWActionModule.h"
#include "Ability/SpellParams.h"
#include "PWAbilityActorModule.generated.h"

struct FPWTargetingResult;

UCLASS(BlueprintType)
class PWGASCORE_API UActorSpellParamsExtension : public USpellParamsExtension
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector SpawnLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRotator SpawnRotation = FRotator::ZeroRotator;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 TargetMask = (int32)ERelationMask::All;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Duration = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bAttachToOwner = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName AttachSocket = NAME_None;
};


UCLASS(Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class PWGASCORE_API UPWAbilityActorModule : public UPWActionModule
{
	GENERATED_BODY()

public:
	/** Actor class to spawn. Must implement PWSpellParamsInterface. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(MustImplement="PWSpellParamsInterface"))
	TSubclassOf<AActor> ActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Bitmask, BitmaskEnum="/Script/PWSharedTypes.ERelationMask"))
	int32 TargetMask = (int32)ERelationMask::All;

	/** If true, the spawned actor will be attached to the ability owner. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bAttachToOwner = false;

	/** Socket name to attach to when bAttachToOwner = true. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SpawnSocket = NAME_None;
	
	/** Optional local offset applied after forward offset. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector SpawnRelativeOffset = FVector::ZeroVector;

	/** If true, pitch will be flattened (useful for ground spawns). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bZeroPitch = false;
	
public:
	virtual USpellParams* MakeSpellParams_Implementation(const FPWTargetingResult& Target) override;

	UFUNCTION(BlueprintCallable, Category="Module")
	virtual AActor* SpawnActor(const FPWTargetingResult& Target);

	UFUNCTION(BlueprintCallable, Category="Module")
	void DestroySpawnedActors();

protected:
	virtual FTransform BuildSpawnTransform(const FPWTargetingResult& Target) const;
	
	TArray<AActor*> SpawnedActors = {};

private:
};