// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PWAbilityModule.generated.h"

class UPWDataModule;
class UPWGameplayAbilityBase;
/**
 * 
 */
UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class PWGASCORE_API UPWAbilityModule : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category="Ability")
	TObjectPtr<UPWGameplayAbilityBase> OwnerAbility = nullptr;

	UFUNCTION(BlueprintCallable, Category="Ability")
	virtual void Initialize(UPWGameplayAbilityBase* InOwner);

	UFUNCTION(BlueprintNativeEvent, Category="Ability")
	void OnAbilityActivated();

	UFUNCTION(BlueprintNativeEvent, Category="Ability")
	void OnAbilityEnded();

	UFUNCTION(BlueprintNativeEvent, Category="Module")
	void GetRequiredDataModules(TArray<TSubclassOf<UPWDataModule>>& OutRequiredModules);
	virtual void GetRequiredDataModules_Implementation(TArray<TSubclassOf<UPWDataModule>>& OutRequiredModules);

protected:
	APlayerController* GetPC() const;
	APawn* GetPawn() const;
};
