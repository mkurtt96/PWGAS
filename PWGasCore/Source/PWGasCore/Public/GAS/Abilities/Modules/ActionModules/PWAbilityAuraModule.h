﻿// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "PWActionModule.h"
#include "Ability/SpellParams.h"
#include "PWAbilityAuraModule.generated.h"

struct FEffectEntry;
class USphereComponent;

UCLASS(Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class PWGASCORE_API UPWAbilityAuraModule : public UPWActionModule
{
	GENERATED_BODY()

public:
	virtual void OnAbilityActivated_Implementation() override;
	virtual void OnAbilityEnded_Implementation() override;
	virtual void GetRequiredDataModules_Implementation(TArray<TSubclassOf<UPWDataModule>>& OutRequiredModules) override;

protected:
	UPROPERTY(Transient)
	TObjectPtr<USphereComponent> AuraSphere = nullptr;

	UPROPERTY(EditAnywhere)
	bool bDrawDebug = false;

	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
							bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void HandleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Applies or removes gameplay effects to overlapping actors
	void ApplyAuraEffect(AActor* Target, bool bApply);
};
