// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "Targeting/Interfaces/PWTargetingInterfaces.h"
#include "UObject/Object.h"
#include "ScreenCenterTargeting.generated.h"

/**
 * 
 */
UCLASS()
class PWGASCORE_API UScreenCenterTargeting : public UObject, public IPWTargetingSource
{
	GENERATED_BODY()

public:
	virtual bool Sample_Implementation(APlayerController* PC, FPWTargetingResult& Out) override
	{
		int32 W, H;
		PC->GetViewportSize(W, H);
		FVector World, Dir;
		if (!UGameplayStatics::DeprojectScreenToWorld(PC, FVector2D(W * 0.5f, H * 0.5f), World, Dir)) return false;

		const FVector Start = World;
		const FVector End = Start + Dir * 100000.f;

		FHitResult Hit;
		PC->GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility);
		const FVector Aim = Hit.bBlockingHit ? Hit.ImpactPoint : End;

		Out.Location = Aim;
		Out.Rotation = Dir.Rotation();
		Out.Hit = Hit;
		Out.bHasHit = Hit.bBlockingHit;
		return true;
	}
};
