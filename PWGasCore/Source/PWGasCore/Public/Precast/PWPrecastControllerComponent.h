// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Precast/Data/PWPrecastTypes.h"
#include "Targeting/Data/PWTargetingTypes.h"
#include "PWPrecastControllerComponent.generated.h"


UCLASS(ClassGroup=(GASCore), meta=(BlueprintSpawnableComponent))
class PWGASCORE_API UPWPrecastControllerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPWPrecastControllerComponent();

	UFUNCTION(BlueprintCallable, Category="GASCore|Precast")
	void StartPrecast(TSubclassOf<UObject> TargetingSourceClass, TSubclassOf<UObject> RangePolicyClass, TSubclassOf<UObject> VisualizerClass, float PreviewRadius, FName OriginSocket, FPWProjectileSimConfig ProjectileConfig);

	UFUNCTION(BlueprintCallable, Category="GASCore|Precast")
	void StopPrecast();

	UFUNCTION(BlueprintPure, Category="GASCore|Precast")
	FVector GetPrecastTargetLocation() const { return Result.Location; }

	UFUNCTION(BlueprintPure, Category="GASCore|Precast")
	FPWTargetingResult GetTargetingResult() const { return Result; }

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY() UObject* TargetingObj = nullptr; // IPWTargetingSource
	UPROPERTY() UObject* RangeObj = nullptr; // IPWRangePolicy
	UPROPERTY() UObject* VisualObj = nullptr; // IPWPrecastVisualizer

	bool bRunning = false;

	// cached authoring
	float PreviewRadius = 120.f;
	FName OriginSocket = NAME_None;
	FPWProjectileSimConfig ProjCfg;

	// live
	FPWTargetingResult Result;

	// helpers
	APlayerController* GetPC() const;
	APawn* GetPawn() const;
	FVector GetOriginLocation() const;
	void BuildProjectileVisual(FPWPrecastVisualUpdate& OutVisualUpdate);
};
