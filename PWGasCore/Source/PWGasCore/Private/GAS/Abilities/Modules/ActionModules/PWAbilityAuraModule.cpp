// copyright mkurtt96


#include "PWGasCore/Public/GAS/Abilities/Modules/ActionModules/PWAbilityAuraModule.h"

#include "AbilitySystemGlobals.h"
#include "Components/SphereComponent.h"
#include "GAS/Abilities/PWGameplayAbilityBase.h"
#include "GAS/Abilities/Modules/DataModules/PWAbilityEffectModule.h"
#include "GAS/Abilities/Modules/DataModules/PWAbilityRangeModule.h"


void UPWAbilityAuraModule::OnAbilityActivated_Implementation()
{
	APawn* OwnerPawn = GetPawn();
	if (!OwnerPawn || !OwnerAbility)
		return;

	UWorld* World = OwnerPawn->GetWorld();
	if (!World)
		return;

	// --- Create sphere component dynamically ---
	AuraSphere = NewObject<USphereComponent>(OwnerPawn, TEXT("AuraSphere"));
	if (!AuraSphere)
		return;

	AuraSphere->RegisterComponent();
	AuraSphere->AttachToComponent(OwnerPawn->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	float Radius = 500.f;
	if (const auto* Range = OwnerAbility->GetRangeModule())
		Radius = Range->GetRangeValue();
	
	AuraSphere->SetSphereRadius(Radius);
	AuraSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	AuraSphere->SetGenerateOverlapEvents(true);

	AuraSphere->OnComponentBeginOverlap.AddDynamic(this, &UPWAbilityAuraModule::HandleBeginOverlap);
	AuraSphere->OnComponentEndOverlap.AddDynamic(this, &UPWAbilityAuraModule::HandleEndOverlap);

	if (bDrawDebug)
		DrawDebugSphere(World, OwnerPawn->GetActorLocation(), Radius, 16, FColor::Green, false, 10.f);

	UE_LOG(LogTemp, Log, TEXT("[AuraModule] Aura activated with radius %.1f"), Radius);
}

void UPWAbilityAuraModule::OnAbilityEnded_Implementation()
{
	if (AuraSphere)
	{
		AuraSphere->DestroyComponent();
		AuraSphere = nullptr;
		UE_LOG(LogTemp, Log, TEXT("[AuraModule] Aura ended, component destroyed."));
	}
}

void UPWAbilityAuraModule::GetRequiredDataModules_Implementation(TArray<TSubclassOf<UPWDataModule>>& OutRequiredModules)
{
	OutRequiredModules.Add(UPWAbilityRangeModule::StaticClass());
	OutRequiredModules.Add(UPWAbilityEffectModule::StaticClass());
}

void UPWAbilityAuraModule::HandleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ApplyAuraEffect(OtherActor, true);
}

void UPWAbilityAuraModule::HandleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ApplyAuraEffect(OtherActor, false);
}

void UPWAbilityAuraModule::ApplyAuraEffect(AActor* Target, bool bApply)
{
	if (!Target || !OwnerAbility)
		return;

	auto* EffectModule = OwnerAbility->GetEffectModule();
	if (!EffectModule)
		return;

	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target);
	UAbilitySystemComponent* SourceASC = OwnerAbility->GetAbilitySystemComponentFromActorInfo();

	if (!TargetASC || !SourceASC)
		return;

	if (bApply)
	{
		EffectModule->ApplyEffects(SourceASC, TargetASC);
		UE_LOG(LogTemp, Verbose, TEXT("[AuraModule] Applied aura effect to %s"), *Target->GetName());
	}
	else
	{
		EffectModule->RemoveEffects(SourceASC, TargetASC);
		UE_LOG(LogTemp, Verbose, TEXT("[AuraModule] Removed aura effect from %s"), *Target->GetName());
	}
}
