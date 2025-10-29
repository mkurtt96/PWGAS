﻿// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Precast/Data/PWPrecastData.h"
#include "Targeting/Data/PWTargetingData.h"
#include "PWPrecastControllerComponent.generated.h"


class UPWTargetingSource;
class UPWRangePolicy;
class UPWPrecastVisualizer;
class UPWTargetResolver;

UCLASS(ClassGroup=(GASCore), meta=(BlueprintSpawnableComponent))
class PWGASCORE_API UPWPrecastControllerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPWPrecastControllerComponent();

	UFUNCTION(BlueprintCallable, Category="GASCore|Precast")
	void StartPrecast(UPWTargetingSource* InTargetingSource, UPWRangePolicy* InRangePolicy, UPWTargetResolver* InTargetResolver, UPWPrecastVisualizer* InVisualizer, float InPreviewRadius, float InPreviewRange, FName InOriginSocket, FPWProjectileSimConfig InProjectileConfig);

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
	UPROPERTY() UPWTargetingSource* TargetingSource = nullptr;
	UPROPERTY() UPWRangePolicy* RangePolicy = nullptr;
	UPROPERTY() UPWPrecastVisualizer* Visualizer = nullptr;
	UPROPERTY() UPWTargetResolver* TargetResolver = nullptr;

	bool bRunning = false;

	// cached authoring
	float PreviewRadius = 0.f;
	float PreviewRange = 0.f;
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
