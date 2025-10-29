// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "Targeting/Data/PWTargetingData.h"
#include "PWGasCore/Public/Targeting/Types/PWTargetPolicyBase.h"
#include "UObject/Object.h"
#include "SingleTargetResolver.generated.h"

/**
 * 
 */
UCLASS()
class PWGASCORE_API USingleTargetResolver : public UPWTargetResolver
{
	GENERATED_BODY()

public:
	virtual void Resolve_Implementation(const AActor* Actor, const FPWTargetingResult& Target, TArray<AActor*>& OutActors) const override
	{
		const FHitResult Hit = Target.Hit;
		if (AActor* HitActor = Hit.GetActor())
			OutActors.Add(HitActor);
	}
};
