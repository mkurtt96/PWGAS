// copyright mkurtt96


#include "Precast/DebugVisualizer/DebugProjectilePathVisualizer.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Precast/Data/PWPrecastData.h"

void UDebugProjectilePathVisualizer::Ensure_Implementation(APlayerController* PC, float PreviewRadius, FName /*OriginSocket*/)
{
	CachedPC    = PC;
	CachedRadius = PreviewRadius;
}

void UDebugProjectilePathVisualizer::Update_Implementation(const FPWPrecastVisualUpdate& Data)
{
	if (!CachedPC.IsValid())
	{
		return;
	}

	UWorld* World = CachedPC->GetWorld();
	if (!World)
	{
		return;
	}

	// --- Draw projectile path ---
	const TArray<FVector>& Points = Data.PathPoints;

	if (Points.Num() > 1)
	{
		for (int32 Index = 1; Index < Points.Num(); ++Index)
		{
			const FVector& From = Points[Index - 1];
			const FVector& To   = Points[Index];

			DrawDebugLine(
				World,
				From,
				To,
				PathColor,
				/*bPersistentLines=*/false,
				/*LifeTime=*/0.0f,    // one frame; we get called every tick
				/*DepthPriority=*/0,
				/*Thickness=*/LineThickness
			);
		}
	}

	// --- Explosion radius at impact point ---
	const FVector ImpactPoint = Data.ImpactPoint;
	const float   Radius      = (Data.Radius > 0.f) ? Data.Radius : CachedRadius;

	if (!ImpactPoint.IsNearlyZero() && Radius > KINDA_SMALL_NUMBER)
	{
		DrawDebugSphere(
			World,
			ImpactPoint,
			Radius,
			SphereSegments,
			ImpactSphereColor,
			/*bPersistentLines=*/false,
			/*LifeTime=*/0.0f,
			/*DepthPriority=*/0,
			/*Thickness=*/LineThickness
		);
	}

	// --- Impact normal visualization ---
	if (!ImpactPoint.IsNearlyZero() && !Data.ImpactNormal.IsNearlyZero())
	{
		const FVector Start = ImpactPoint;
		const FVector End   = Start + Data.ImpactNormal.GetSafeNormal() * ImpactNormalLength;

		DrawDebugLine(
			World,
			Start,
			End,
			ImpactNormalColor,
			/*bPersistentLines=*/false,
			/*LifeTime=*/0.0f,
			/*DepthPriority=*/0,
			/*Thickness=*/LineThickness
		);
	}
}

void UDebugProjectilePathVisualizer::Hide_Implementation()
{
	// Nothing to do for debug lines with LifeTime=0:
	// they only live for a single frame and will stop appearing
	// as soon as Update() stops being called.
	CachedPC = nullptr;
}