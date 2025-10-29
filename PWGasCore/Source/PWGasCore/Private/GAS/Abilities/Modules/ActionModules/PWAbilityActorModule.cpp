// copyright mkurtt96


#include "PWGasCore/Public/GAS/Abilities/Modules/ActionModules/PWAbilityActorModule.h"

#include "Ability/PWSpellParamsInterface.h"
#include "GAS/Abilities/PWGameplayAbilityBase.h"


USpellParams* UPWAbilityActorModule::MakeSpellParams_Implementation(const FPWTargetingResult& Target)
{
	if (!OwnerAbility)
		return nullptr;

	USpellParams* Params = OwnerAbility->MakeSpellParams();
	UActorSpellParamsExtension* ActorParams = Params->AddExtension<UActorSpellParamsExtension>();
	ActorParams->SpawnLocation = Target.Location;
	ActorParams->SpawnRotation = (Target.Location - OwnerAbility->GetPawn()->GetActorLocation()).Rotation();
	ActorParams->bAttachToOwner = bAttachToOwner;
	ActorParams->AttachSocket = SpawnSocket;
	Params->TargetCollisionMask = TargetMask;

	ExtendParamsWithData(Params);
	
	return Params;
}

AActor* UPWAbilityActorModule::SpawnActor(const FPWTargetingResult& Target)
{
	if (!ActorClass || !OwnerAbility)
		return nullptr;

	AActor* Avatar = OwnerAbility->GetAvatarActorFromActorInfo();
	if (!Avatar || !Avatar->HasAuthority())
		return nullptr;

	UWorld* World = Avatar->GetWorld();
	if (!World)
		return nullptr;

	FTransform SpawnTM = BuildSpawnTransform(Target);

	AActor* Spawned = World->SpawnActorDeferred<AActor>(
		ActorClass, SpawnTM, Avatar, Cast<APawn>(Avatar),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);
	if (!Spawned)
		return nullptr;

	if (!Spawned->GetClass()->ImplementsInterface(UPWSpellParamsInterface::StaticClass()))
	{
		UE_LOG(LogTemp, Error, TEXT("%s must implement IPWSpellParamsInterface (ActorModule)."), *ActorClass->GetName());
		Spawned->Destroy();
		return nullptr;
	}

	if (USpellParams* ParamsObj = MakeSpellParams(Target))
	{
		IPWSpellParamsInterface::Execute_SetSpellParams(Spawned, ParamsObj);
	}
	
	SpawnedActors.Add(Spawned);

	Spawned->FinishSpawning(SpawnTM);

	if (bAttachToOwner && Avatar)
	{
		if (SpawnSocket != NAME_None)
		{
			if (USkeletalMeshComponent* Mesh = Avatar->FindComponentByClass<USkeletalMeshComponent>())
			{
				if (Mesh->DoesSocketExist(SpawnSocket))
					Spawned->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SpawnSocket);
				else
					UE_LOG(LogTemp, Warning, TEXT("Attach socket '%s' missing on %s."), *SpawnSocket.ToString(), *Avatar->GetName());
			}
		}
		else
		{
			Spawned->AttachToActor(Avatar, FAttachmentTransformRules::KeepWorldTransform);
		}
	}

	return Spawned;
}

void UPWAbilityActorModule::DestroySpawnedActors()
{
	for (AActor* SpawnedActor : SpawnedActors)
	{
		SpawnedActor->Destroy();
	}
	SpawnedActors.Empty();
}

FTransform UPWAbilityActorModule::BuildSpawnTransform(const FPWTargetingResult& Target) const
{
	FVector Loc;
	FRotator Rot;

	AActor* Avatar = OwnerAbility ? OwnerAbility->GetAvatarActorFromActorInfo() : nullptr;
	if (!Avatar)
		return FTransform::Identity;

	if (SpawnSocket != NAME_None)
	{
		if (const APawn* Pawn = OwnerAbility->GetPawn())
		{
			if (const USkeletalMeshComponent* Mesh = Pawn->FindComponentByClass<USkeletalMeshComponent>())
			{
				if (Mesh->DoesSocketExist(SpawnSocket))
				{
					Loc = Mesh->GetSocketLocation(SpawnSocket);
					Rot = (Target.Location - Loc).Rotation();
				}
			}
		}
	}
	else
	{
		Loc = Avatar->GetActorLocation();
		Rot = (Target.Location - Loc).Rotation();
	}

	if (bZeroPitch)
		Rot.Pitch = 0.f;

	Loc += SpawnRelativeOffset;
	return FTransform(Rot, Loc);
}