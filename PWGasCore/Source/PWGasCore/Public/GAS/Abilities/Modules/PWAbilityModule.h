// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/PWGameplayAbilityBase.h"
#include "UObject/Object.h"
#include "PWAbilityModule.generated.h"

class UPWGameplayAbilityBase;
/**
 * 
 */
UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class PWGASCORE_API UPWAbilityModule : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(EditCondition=false, ToolTip="Index among modules of the same class type in AbilityModules."))
	int32 ModuleIndex = INDEX_NONE;
	
	UPROPERTY(BlueprintReadOnly, Category="GASCore|AbilityComponent")
	TObjectPtr<UPWGameplayAbilityBase> OwnerAbility = nullptr;

	UFUNCTION(BlueprintCallable, Category="GASCore|AbilityComponent")
	virtual void Initialize(UPWGameplayAbilityBase* InOwner);

	UFUNCTION(BlueprintNativeEvent, Category="GASCore|AbilityComponent")
	void OnAbilityActivated();
	virtual void OnAbilityActivated_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, Category="GASCore|AbilityComponent")
	void OnAbilityEnded();
	virtual void OnAbilityEnded_Implementation() {}

	template <typename T>
	T* GetLinkedModule() const
	{
		if (!OwnerAbility)
			return nullptr;

		TArray<T*> Matches;
		for (UPWAbilityModule* M : OwnerAbility->AbilityModules)
			if (T* AsType = Cast<T>(M))
				Matches.Add(AsType);

		if (Matches.Num() == 0)
			return nullptr;

		// only one found
		if (Matches.Num() == 1)
			return Matches[0];

		// multiple, match by ModuleTypeIndex
		for (T* M : Matches)
			if (M && M->ModuleIndex == ModuleIndex)
				return M;

		UE_LOG(LogTemp, Warning,
			TEXT("[%s] found %d %s modules but none match index %d."),
			*GetNameSafe(this), Matches.Num(), *T::StaticClass()->GetName(), ModuleIndex);

		return nullptr;
	}
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
protected:
	APlayerController* GetPC() const;
	APawn* GetPawn() const;
};
