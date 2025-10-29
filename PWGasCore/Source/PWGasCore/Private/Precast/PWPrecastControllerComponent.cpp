// copyright mkurtt96


#include "Precast/PWPrecastControllerComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "Precast/Data/PWPrecastTypes.h"
#include "Targeting/Data/PWTargetingData.h"
#include "PWGasCore/Public/Targeting/Types/PWTargetPolicyBase.h"

UPWPrecastControllerComponent::UPWPrecastControllerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPWPrecastControllerComponent::StartPrecast(UPWTargetingSource* InTargetingSource, UPWRangePolicy* InRangePolicy, UPWTargetResolver* InTargetResolver, UPWPrecastVisualizer* InVisualizer, float InPreviewRadius, float InPreviewRange, FName InOriginSocket, FPWProjectileSimConfig InProjectileConfig)
{
	APlayerController* PC = GetPC();
	if (!PC || !PC->IsLocalController()) return;

	StopPrecast();

	TargetingSource = InTargetingSource;
	RangePolicy = InRangePolicy;
	TargetResolver = InTargetResolver;
	Visualizer = InVisualizer;
	
	PreviewRadius = InPreviewRadius;
	PreviewRange = InPreviewRange;
	OriginSocket = InOriginSocket;
	ProjCfg = InProjectileConfig;

	if (Visualizer)
		Visualizer->Ensure(PC, PreviewRadius, OriginSocket);

	bRunning = true;
	SetComponentTickEnabled(true);
}

void UPWPrecastControllerComponent::StopPrecast()
{
	SetComponentTickEnabled(false);
	Visualizer->Hide();
	
	TargetingSource = nullptr;
	RangePolicy = nullptr;
	TargetResolver = nullptr;
	Visualizer = nullptr;
	
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

	if (TargetingSource) TargetingSource->Sample(PC, Result);
	if (RangePolicy) RangePolicy->Apply(GetPawn(), PreviewRange, Result);
	TArray<AActor*> OutActors;
	if (TargetResolver) TargetResolver->Resolve(PC, Result, OutActors);
	if (Visualizer)
	{
		FPWPrecastVisualUpdate VisualUpdate;
		VisualUpdate.Radius = PreviewRadius;
		VisualUpdate.ImpactPoint = Result.Location;
		VisualUpdate.ImpactNormal = FVector::UpVector;

		// projectile path computation
		BuildProjectileVisual(VisualUpdate);
		Visualizer->Update(VisualUpdate);
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
