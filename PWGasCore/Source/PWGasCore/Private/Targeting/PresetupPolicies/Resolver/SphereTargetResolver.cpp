// copyright mkurtt96


#include "Targeting/PresetupPolicies/Resolver/SphereTargetResolver.h"

#include "Engine/OverlapResult.h"
#include "GAS/Abilities/PWGameplayAbilityBase.h"
#include "GAS/Abilities/Modules/DataModules/PWAbilityRadiusModule.h"

void USphereTargetResolver::Initialize_Implementation(UPWGameplayAbilityBase* InOwner)
{
	Super::Initialize_Implementation(InOwner);
	Radius = OwnerAbility->GetRadiusModule()->GetRadiusValue();
}

void USphereTargetResolver::Resolve_Implementation(const AActor* Actor, const FPWTargetingResult& Target, TArray<AActor*>& OutActors) const
{
	if (!Actor || !Actor->GetWorld()) return;

	TArray<FOverlapResult> Overlaps;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);

	if (Actor->GetWorld()->OverlapMultiByObjectType(Overlaps, Target.Location, FQuat::Identity, FCollisionObjectQueryParams(ECC_Pawn), Sphere))
	{
		for (auto& R : Overlaps)
			if (AActor* A = R.GetActor())
				OutActors.Add(A);
	}
}

void USphereTargetResolver::GetRequiredDataModules_Implementation(TArray<TSubclassOf<UPWDataModule>>& OutRequiredModules)
{
	Super::GetRequiredDataModules_Implementation(OutRequiredModules);

	OutRequiredModules.Add(UPWAbilityRadiusModule::StaticClass());
}
