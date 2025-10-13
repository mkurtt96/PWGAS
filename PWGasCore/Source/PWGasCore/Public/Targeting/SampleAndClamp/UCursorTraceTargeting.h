// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "Targeting/Data/PWTargetingTypes.h"
#include "Targeting/Interfaces/PWTargetingInterfaces.h"
#include "UObject/Object.h"
#include "UCursorTraceTargeting.generated.h"

/**
 * 
 */
UCLASS()
class PWGASCORE_API UUCursorTraceTargeting : public UObject, public IPWTargetingSource
{
	GENERATED_BODY()
public:
	virtual bool Sample_Implementation(APlayerController* PC, FPWTargetingResult& Out) override
	{
		FHitResult Hit;
		PC->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), false, Hit);
		if (!Hit.bBlockingHit) return false;

		Out.Location  = Hit.ImpactPoint;
		const APawn* P = PC->GetPawn();
		Out.Rotation  = P ? (Hit.ImpactPoint - P->GetActorLocation()).Rotation() : FRotator::ZeroRotator;
		Out.Hit = Hit;
		Out.bHasHit = true;
		return true;
	}
};