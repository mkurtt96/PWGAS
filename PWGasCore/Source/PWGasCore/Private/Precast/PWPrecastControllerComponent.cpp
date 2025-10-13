// copyright mkurtt96


#include "Precast/PWPrecastControllerComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "Precast/Data/PWPrecastInterfaces.h"
#include "Targeting/Interfaces/PWTargetingInterfaces.h"
#include "Targeting/Data/PWTargetingTypes.h"

UPWPrecastControllerComponent::UPWPrecastControllerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPWPrecastControllerComponent::StartPrecast(TSubclassOf<UObject> TargetingSourceClass, TSubclassOf<UObject> RangePolicyClass, TSubclassOf<UObject> VisualizerClass, float InPreviewRadius, FName InOriginSocket, FPWProjectileSimConfig InProjectileConfig)
{
	APlayerController* PC = GetPC();
	if (!PC || !PC->IsLocalController()) return;

	StopPrecast();

	PreviewRadius = InPreviewRadius;
	OriginSocket = InOriginSocket;
	ProjCfg = InProjectileConfig;

	if (TargetingSourceClass)
		TargetingObj = NewObject<UObject>(this, TargetingSourceClass);
	if (RangePolicyClass)
		RangeObj = NewObject<UObject>(this, RangePolicyClass);
	if (VisualizerClass)
		VisualObj = NewObject<UObject>(this, VisualizerClass);

	if (IPWPrecastVisualizer* PrecastVisualizer = Cast<IPWPrecastVisualizer>(VisualObj))
	{
		PrecastVisualizer->Execute_Ensure(VisualObj, PC, PreviewRadius, OriginSocket);
	}

	bRunning = true;
	SetComponentTickEnabled(true);
}

void UPWPrecastControllerComponent::StopPrecast()
{
	SetComponentTickEnabled(false);
	if (IPWPrecastVisualizer* PrecastVisualizer = Cast<IPWPrecastVisualizer>(VisualObj))
	{
		PrecastVisualizer->Execute_Hide(VisualObj);
	}
	TargetingObj = RangeObj = VisualObj = nullptr;
	Result = FPWTargetingResult();
	bRunning = false;
}

void UPWPrecastControllerComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
}

void UPWPrecastControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!bRunning) return;
	APlayerController* PC = GetPC();
	if (!PC)
	{
		StopPrecast();
		return;
	}

	// 1) Targeting
	if (const IPWTargetingSource* TargetingSource = Cast<IPWTargetingSource>(TargetingObj))
	{
		if (!TargetingSource->Execute_Sample(TargetingObj, PC, Result)) return;
	}

	// 2) Range clamp (optional)
	if (const IPWRangePolicy* RangePolicy = Cast<IPWRangePolicy>(RangeObj))
	{
		RangePolicy->Execute_Apply(RangeObj, GetPawn(), PreviewRadius, Result);
	}
	
	if (const IPWPrecastVisualizer* PrecastVisualizer = Cast<IPWPrecastVisualizer>(VisualObj))
	{
		// 3) Visual update payload
		FPWPrecastVisualUpdate VisualUpdate;
		VisualUpdate.Radius = PreviewRadius;
		VisualUpdate.ImpactPoint = Result.Location;
		VisualUpdate.ImpactNormal = FVector::UpVector;

		// 3a) If visualizer wants projectile path, it’ll use PathPoints (we’ll compute it here)
		BuildProjectileVisual(VisualUpdate);

		// 4) Push
		PrecastVisualizer->Execute_Update(VisualObj, VisualUpdate);
	}
}

APlayerController* UPWPrecastControllerComponent::GetPC() const
{
	return Cast<APlayerController>(GetOwner());
}

APawn* UPWPrecastControllerComponent::GetPawn() const
{
	return GetPC() ? GetPC()->GetPawn() : nullptr;
}

FVector UPWPrecastControllerComponent::GetOriginLocation() const
{
	const APawn* P = GetPawn();
	if (!P) return FVector::ZeroVector;

	if (!OriginSocket.IsNone())
	{
		if (const USkeletalMeshComponent* Skel = P->FindComponentByClass<USkeletalMeshComponent>())
		{
			return Skel->GetSocketLocation(OriginSocket);
		}
	}
	const USceneComponent* Root = P->GetRootComponent();
	return Root ? Root->GetComponentLocation() : P->GetActorLocation();
}

void UPWPrecastControllerComponent::BuildProjectileVisual(FPWPrecastVisualUpdate& OutVisualUpdate)
{
	const FVector Start = GetOriginLocation();
	const FVector AimDir = (Result.Location - Start).GetSafeNormal();

	FPredictProjectilePathParams Params;  
	Params.bTraceWithCollision = true;
	Params.ProjectileRadius = ProjCfg.ProjectileRadius;
	Params.MaxSimTime = ProjCfg.MaxSimTime;
	Params.SimFrequency = ProjCfg.SimFrequency;
	Params.TraceChannel = ProjCfg.TraceChannel;
	Params.DrawDebugType = EDrawDebugTrace::None;
	if (!FMath::IsNearlyZero(ProjCfg.GravityZOverride))
		Params.OverrideGravityZ = ProjCfg.GravityZOverride;

	Params.StartLocation = Start;
	Params.LaunchVelocity = AimDir * ProjCfg.InitialSpeed;

	FPredictProjectilePathResult PathResult;
	UGameplayStatics::PredictProjectilePath(GetWorld(), Params, PathResult);

	OutVisualUpdate.PathPoints.Reset();
	for (const auto& PathPointData : PathResult.PathData)
		OutVisualUpdate.PathPoints.Add(PathPointData.Location);

	const bool bHit = PathResult.HitResult.bBlockingHit;
	Result.bHasHit = bHit;
	Result.Hit = PathResult.HitResult;

	OutVisualUpdate.ImpactPoint = bHit ? PathResult.HitResult.ImpactPoint : (OutVisualUpdate.PathPoints.Num() ? OutVisualUpdate.PathPoints.Last() : Result.Location);
	OutVisualUpdate.ImpactNormal = (bHit && ProjCfg.bAlignToGroundNormal) ? PathResult.HitResult.ImpactNormal : FVector::UpVector;
}
