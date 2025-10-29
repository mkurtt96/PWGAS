// copyright mkurtt96


#include "GAS/Abilities/Modules/ActionModules/PWAbilityProjectileModule.h"

#include "GAS/Abilities/PWGameplayAbilityBase.h"

USpellParams* UPWAbilityProjectileModule::MakeSpellParams_Implementation(const FPWTargetingResult& Target)
{
	if (!OwnerAbility)
		return nullptr;

	USpellParams* Params = OwnerAbility->MakeSpellParams();
	UProjectileSpellParamsExtension* ProjectileParams = Params->AddExtension<UProjectileSpellParamsExtension>();
	ProjectileParams->SpawnLocation = Target.Location;
	ProjectileParams->SpawnRotation = (Target.Location - OwnerAbility->GetPawn()->GetActorLocation()).Rotation();
	ProjectileParams->bAttachToOwner = bAttachToOwner;
	ProjectileParams->AttachSocket = SpawnSocket;
	
	ProjectileParams->InitialSpeed = ProjectileConfig.InitialSpeed;
	ProjectileParams->GravityZOverride = ProjectileConfig.GravityZOverride;
	ProjectileParams->ProjectileRadius = ProjectileConfig.ProjectileRadius;

	ExtendParamsWithData(Params);

	return Params;
}

AActor* UPWAbilityProjectileModule::SpawnProjectile(const FPWTargetingResult& Target)
{
	return SpawnActor(Target);
}

const FPWProjectileSimConfig& UPWAbilityProjectileModule::GetProjectileConfig() const
{ return ProjectileConfig; }
