// copyright mkurtt96

#include "GAS/Abilities/Modules/ActionModules/PWAbilityMultiProjectileModule.h"

#include "Algo/RandomShuffle.h"
#include "GAS/Abilities/PWGameplayAbilityBase.h"


AActor* UPWAbilityMultiProjectileModule::SpawnProjectile(const FPWTargetingResult& Target)
{
	if (ProjectileCount <= 1)
	{
		return Super::SpawnProjectile(Target);
	}
	
	SpreadSpawnProjectiles(Target);
	return nullptr;
}

void UPWAbilityMultiProjectileModule::SpreadSpawnProjectiles(const FPWTargetingResult& Target)
{
	if (ProjectileCount <= 1)
	{
		Super::SpawnProjectile(Target);
		return;
	}

	BuildSpreadOrder();
	CurrentSpreadIndex = 0;

	SpreadSpawnOneProjectile(Target);

	// Schedule remaining projectiles if there's a delay
	if (SpreadDelay > 0.f && ProjectileCount > 1)
	{
		FTimerHandle SpreadTimerHandle;
		OwnerAbility->GetWorld()->GetTimerManager().SetTimer(
			SpreadTimerHandle,
			[this, Target]() { SpreadSpawnOneProjectile(Target); },
			SpreadDelay,
			true
		);
	}
	else
	{
		for (int32 i = 1; i < ProjectileCount; ++i)
		{
			SpreadSpawnOneProjectile(Target);
		}
	}
}

void UPWAbilityMultiProjectileModule::SpreadSpawnOneProjectile(const FPWTargetingResult& Target)
{
	if (CurrentSpreadIndex >= CachedSpreadOrder.Num())
		return;

	FPWTargetingResult SpreadTarget = Target;
	
	if (ProjectileCount > 1)
	{
		int32 SpreadIndex = CachedSpreadOrder[CurrentSpreadIndex];
		FRotator SpreadRotation = ComputeSpreadRotation(SpreadIndex);
		SpreadTarget.Rotation = SpreadRotation + Target.Rotation;
	}

	Super::SpawnProjectile(SpreadTarget);
	CurrentSpreadIndex++;

	// Clear timer if we've spawned all projectiles
	if (CurrentSpreadIndex >= ProjectileCount)
	{
		// Timer will naturally complete, no need to clear manually
	}
}

void UPWAbilityMultiProjectileModule::BuildSpreadOrder()
{
	CachedSpreadOrder.Empty(ProjectileCount);
	
	switch (SpreadPattern)
	{
	case EProjectileSpreadPattern::LeftToRight: Build_LeftToRight(); break;
	case EProjectileSpreadPattern::RightToLeft: Build_RightToLeft(); break;
	case EProjectileSpreadPattern::CenterOut: Build_CenterOut(); break;
	case EProjectileSpreadPattern::OutIn: Build_OutIn(); break;
	case EProjectileSpreadPattern::CenterFanOutRight: Build_CenterFanOutRight(); break;
	case EProjectileSpreadPattern::CenterFanOutLeft: Build_CenterFanOutLeft(); break;
	case EProjectileSpreadPattern::OutFanInRight: Build_OutFanInRight(); break;
	case EProjectileSpreadPattern::OutFanInLeft: Build_OutFanInLeft(); break;
	case EProjectileSpreadPattern::Random2D: Build_Random(false); break;
	case EProjectileSpreadPattern::Random3D: Build_Random(true); break;
	default: Build_LeftToRight(); break;
	}
}

FRotator UPWAbilityMultiProjectileModule::ComputeSpreadRotation(int32 SpreadIndex) const
{
	const float HalfSpread = SpreadAngle * 0.5f;
	float OffsetYaw = 0.f;

	if (ProjectileCount > 1)
		OffsetYaw = FMath::Lerp(-HalfSpread, +HalfSpread, SpreadIndex / float(ProjectileCount - 1));

	FRotator Rot(0, OffsetYaw, 0);
	if (SpreadPattern == EProjectileSpreadPattern::Random3D)
		Rot.Pitch = FMath::FRandRange(-HalfSpread, +HalfSpread);

	return Rot;
}

void UPWAbilityMultiProjectileModule::Build_LeftToRight()
{
	for (int32 i = 0; i < ProjectileCount; ++i)
		CachedSpreadOrder.Add(i);
}

void UPWAbilityMultiProjectileModule::Build_RightToLeft()
{
	for (int32 i = ProjectileCount - 1; i >= 0; --i)
		CachedSpreadOrder.Add(i);
}

void UPWAbilityMultiProjectileModule::Build_CenterOut()
{
	const int32 Mid = ProjectileCount / 2;
	if (ProjectileCount % 2 == 1)
		CachedSpreadOrder.Add(Mid);
	for (int32 i = 1; i <= Mid; ++i)
	{
		int32 L = Mid - i;
		int32 R = Mid + i;
		if (R < ProjectileCount) CachedSpreadOrder.Add(R);
		if (L >= 0) CachedSpreadOrder.Add(L);
	}
}

void UPWAbilityMultiProjectileModule::Build_OutIn()
{
	const int32 Mid = ProjectileCount / 2;
	for (int32 i = Mid; i > 0; --i)
	{
		int32 L = Mid - i;
		int32 R = ProjectileCount - L - 1;
		if (L >= 0) CachedSpreadOrder.Add(L);
		if (R < ProjectileCount) CachedSpreadOrder.Add(R);
	}
	if (ProjectileCount % 2 == 1)
		CachedSpreadOrder.Add(Mid);
}

void UPWAbilityMultiProjectileModule::Build_CenterFanOutRight()
{
	const int32 Mid = ProjectileCount / 2;
	if (ProjectileCount % 2 == 1)
		CachedSpreadOrder.Add(Mid);
	for (int32 i = 1; i <= Mid; ++i)
	{
		int32 R = Mid + i;
		int32 L = Mid - i;
		if (R < ProjectileCount) CachedSpreadOrder.Add(R);
		if (L >= 0) CachedSpreadOrder.Add(L);
	}
}

void UPWAbilityMultiProjectileModule::Build_CenterFanOutLeft()
{
	const int32 Mid = ProjectileCount / 2;
	if (ProjectileCount % 2 == 1)
		CachedSpreadOrder.Add(Mid);
	for (int32 i = 1; i <= Mid; ++i)
	{
		int32 L = Mid - i;
		int32 R = Mid + i;
		if (L >= 0) CachedSpreadOrder.Add(L);
		if (R < ProjectileCount) CachedSpreadOrder.Add(R);
	}
}

void UPWAbilityMultiProjectileModule::Build_OutFanInRight()
{
	for (int32 i = 0; i < ProjectileCount / 2; ++i)
	{
		int32 R = ProjectileCount - 1 - i;
		int32 L = i;
		if (R < ProjectileCount) CachedSpreadOrder.Add(R);
		if (L >= 0) CachedSpreadOrder.Add(L);
	}
	if (ProjectileCount % 2 == 1)
		CachedSpreadOrder.Add(ProjectileCount / 2);
}

void UPWAbilityMultiProjectileModule::Build_OutFanInLeft()
{
	for (int32 i = 0; i < ProjectileCount / 2; ++i)
	{
		int32 L = i;
		int32 R = ProjectileCount - 1 - i;
		if (L >= 0) CachedSpreadOrder.Add(L);
		if (R < ProjectileCount) CachedSpreadOrder.Add(R);
	}
	if (ProjectileCount % 2 == 1)
		CachedSpreadOrder.Add(ProjectileCount / 2);
}

void UPWAbilityMultiProjectileModule::Build_Random(bool bThreeDimensional)
{
	for (int32 i = 0; i < ProjectileCount; ++i)
		CachedSpreadOrder.Add(i);
	Algo::RandomShuffle(CachedSpreadOrder);

	if (bThreeDimensional)
	{
		for (int32& Idx : CachedSpreadOrder)
			Idx += FMath::RandRange(-2, 2); // variation on pitch later
	}
}
