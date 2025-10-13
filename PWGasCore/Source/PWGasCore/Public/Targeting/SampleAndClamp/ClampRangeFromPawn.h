// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "Targeting/Data/PWTargetingTypes.h"
#include "Targeting/Interfaces/PWTargetingInterfaces.h"
#include "UObject/Object.h"
#include "ClampRangeFromPawn.generated.h"

/**
 * 
 */
UCLASS()
class PWGASCORE_API UClampRangeFromPawn : public UObject, public IPWRangePolicy
{
	GENERATED_BODY()

public:
	virtual void Apply_Implementation(const AActor* Actor, const float MaxRange, FPWTargetingResult& InOut) override
	{
		if (!Actor || MaxRange <= 0.f) return;

		const FVector Origin = Actor->GetActorLocation();
		const FVector Target = InOut.Location;

		const FVector Dir = (Target - Origin).GetSafeNormal();
		const float Dist = FVector::Dist(Origin, Target);

		if (Dist > MaxRange) InOut.Location = Origin + Dir * MaxRange;
		InOut.Rotation = (InOut.Location - Actor->GetActorLocation()).Rotation();
	}
};
