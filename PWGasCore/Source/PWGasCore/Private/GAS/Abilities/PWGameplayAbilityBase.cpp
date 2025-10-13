// copyright mkurtt96


#include "GAS/Abilities/PWGameplayAbilityBase.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagsManager.h"
#include "GameplayTagsSettings.h"
#include "GAS/Abilities/Modules/PWAbilityTargetingModule.h"
#include "PWGasCore/Public/GAS/Abilities/Modules/PWAbilityModule.h"
#include "GAS/ASC/PWAbilitySystemComponent.h"
#include "GAS/ASC/PWASC_InputBinding.h"
#include "GAS/Data/FPWAbilityInputListener.h"
#include "GAS/Tags/GASCoreTags.h"
#include "Precast/PWPrecastControllerComponent.h"
#include "Targeting/Data/PWTargetingTypes.h"
#include "Targeting/Interfaces/PWTargetingInterfaces.h"
#include "VisualAndAudio/PWAnimSetProvider.h"


APlayerController* UPWGameplayAbilityBase::GetPC() const
{
	if (const FGameplayAbilityActorInfo* Info = GetCurrentActorInfo())
		return Cast<APlayerController>(Info->PlayerController.Get());
	return nullptr;
}

APawn* UPWGameplayAbilityBase::GetPawn() const
{
	if (const FGameplayAbilityActorInfo* Info = GetCurrentActorInfo())
		return Cast<APawn>(Info->AvatarActor.Get());
	return nullptr;
}

void UPWGameplayAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	for (UPWAbilityModule* Comp : AbilityModules)
	{
		if (Comp)
		{
			Comp->Initialize(this);
			Comp->OnAbilityActivated();
		}
	}
}

UPWAbilityModule* UPWGameplayAbilityBase::GetModuleByClass(const TSubclassOf<UPWAbilityModule> ComponentClass) const
{
	for (UPWAbilityModule* Comp : AbilityModules)
	{
		if (Comp && Comp->IsA(ComponentClass))
			return Comp;
	}
	return nullptr;
}

FPWTargetingResult UPWGameplayAbilityBase::ComputeTargetOnce() const
{
	if (UPWAbilityTargetingModule* TMod = GetModule<UPWAbilityTargetingModule>())
	{
		FPWTargetingResult R;
		TMod->ComputeTarget(R);
		return R;
	}
	return FPWTargetingResult();
}

FGameplayAbilityTargetDataHandle UPWGameplayAbilityBase::MakeTargetDataFromResult(const FPWTargetingResult& Result) const
{
	if (Result.bHasHit && Result.Hit.bBlockingHit)
	{
		FGameplayAbilityTargetData_SingleTargetHit* HitData = new FGameplayAbilityTargetData_SingleTargetHit(Result.Hit);
		return FGameplayAbilityTargetDataHandle(HitData);
	}

	FGameplayAbilityTargetData_LocationInfo* LocData = new FGameplayAbilityTargetData_LocationInfo();
	LocData->TargetLocation.LocationType = EGameplayAbilityTargetingLocationType::LiteralTransform;
	LocData->TargetLocation.LiteralTransform = FTransform(Result.Rotation, Result.Location);
	return FGameplayAbilityTargetDataHandle(LocData);
}

FVector UPWGameplayAbilityBase::ExtractLocation(const FGameplayAbilityTargetData* TD) const
{
	auto ExtractLocation = [](const FGameplayAbilityTargetData* TD) -> TOptional<FVector>
	{
		if (const auto* HitTD = static_cast<const FGameplayAbilityTargetData_SingleTargetHit*>(TD))
		{
			if (HitTD->HitResult.IsValidBlockingHit())
			{
				return HitTD->HitResult.ImpactPoint;
			}
		}
		else if (const auto* LocTD = static_cast<const FGameplayAbilityTargetData_LocationInfo*>(TD))
		{
			const FTransform Xform = LocTD->TargetLocation.GetTargetingTransform();
			return Xform.GetLocation();
		}

		return {};
	};
	return {};
}

float UPWGameplayAbilityBase::GetCooldown(int32 Level) const
{
	return Cooldown.GetValueAtLevel(Level);
}

UAnimMontage* UPWGameplayAbilityBase::GetAnimationMontageFromActor() const
{
	const AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor || !AnimationTag.IsValid())
		return nullptr;

	if (AvatarActor->GetClass()->ImplementsInterface(UPWAnimSetProvider::StaticClass()))
	{
		UAnimMontage* MontageForTag = IPWAnimSetProvider::Execute_GetMontageForTag(AvatarActor, AnimationTag);
		return MontageForTag;
	}

	UE_LOG(LogTemp, Warning, TEXT("AvatarActor: %s does not use the IPWAnimSetProvider, See UPWGameplayAbilityBase::GetAnimationMontageFromActor"), *AvatarActor->GetName())
	return nullptr;
}

FGameplayTagContainer UPWGameplayAbilityBase::GetCooldownGameplayTags() const
{
	FGameplayTagContainer MutableTags = FGameplayTagContainer();
	MutableTags.Reset();

	if (const FGameplayTagContainer* Parent = Super::GetCooldownTags())
		MutableTags.AppendTags(*Parent);

	if (AbilityTag.IsValid())
	{
		const FGameplayTag CooldownTag = PWTags::Ability::Spell::GetCooldownTag(AbilityTag);
		if (CooldownTag.IsValid())
			MutableTags.AddTag(CooldownTag);
	}

	return MutableTags;
}

void UPWGameplayAbilityBase::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	if (const UGameplayEffect* CooldownGE = GetCooldownGameplayEffect())
	{
		const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CooldownGE->GetClass(), GetAbilityLevel());
		const FGameplayTag CooldownTag = PWTags::Ability::Spell::GetCooldownTag(AbilityTag);
		SpecHandle.Data.Get()->DynamicGrantedTags.AppendTags(CooldownTag.GetSingleTagContainer());
		SpecHandle.Data.Get()->SetSetByCallerMagnitude(PWTags::Ability::Cooldown, Cooldown.GetValueAtLevel(GetAbilityLevel()));
		ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
	}
}

void UPWGameplayAbilityBase::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UPWGameplayAbilityBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	for (UPWAbilityModule* Comp : AbilityModules)
	{
		if (Comp)
			Comp->OnAbilityEnded();
	}
}

void UPWGameplayAbilityBase::HandleTaggedAbilityInput_Implementation(const EPWInputEventType& InputType, const FGameplayTag& InputTag)
{
	if (const FGameplayTagContainer* BoundTags = InputTagBindings.Find(InputType))
	{
		if (BoundTags->HasTagExact(InputTag))
		{
			switch (InputType)
			{
			case EPWInputEventType::Pressed:
				OnInputPressed(InputTag);
				break;
			case EPWInputEventType::Released:
				OnInputReleased(InputTag);
				break;
			}
		}
	}
}

void UPWGameplayAbilityBase::GetHandledInputTags_Implementation(TArray<FGameplayTag>& OutTags) const
{
}

void UPWGameplayAbilityBase::RegisterAbilityInputListener_Implementation(const TArray<FPWAbilityInputListener>& Listeners, const bool bExclusive)
{
	if (UPWAbilitySystemComponent* ASC = Cast<UPWAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo()))
	{
		ASC->Input().RegisterAbilityInputListener(GetCurrentAbilitySpecHandle(), Listeners, bExclusive);
	}
}

void UPWGameplayAbilityBase::UnregisterAbilityInputListener_Implementation()
{
	if (UPWAbilitySystemComponent* ASC = Cast<UPWAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo()))
	{
		ASC->Input().UnregisterAbilityInputListener(GetCurrentAbilitySpecHandle());
	}
}

UPWAbilityActorModule* UPWGameplayAbilityBase::GetActorModule(int32 Index) const
{
	return GetModule<UPWAbilityActorModule>(Index);
}

UPWAbilityProjectileModule* UPWGameplayAbilityBase::GetProjectileModule(int32 Index) const
{
	return GetModule<UPWAbilityProjectileModule>(Index);
}

UPWAbilityPrecastModule* UPWGameplayAbilityBase::GetPrecastModule(int32 Index) const
{
	return GetModule<UPWAbilityPrecastModule>(Index);
}

UPWAbilityEffectModule* UPWGameplayAbilityBase::GetEffectModule(int32 Index) const
{
	return GetModule<UPWAbilityEffectModule>(Index);
}

UPWAbilityAuraModule* UPWGameplayAbilityBase::GetAuraModule(int32 Index) const
{
	return GetModule<UPWAbilityAuraModule>(Index);
}

UPWAbilityTargetingModule* UPWGameplayAbilityBase::GetTargetingModule(int32 Index) const
{
	return GetModule<UPWAbilityTargetingModule>(Index);
}

UPWAbilityRangeModule* UPWGameplayAbilityBase::GetRangeModule(int32 Index) const
{
	return GetModule<UPWAbilityRangeModule>(Index);
}

#if WITH_EDITOR

void UPWGameplayAbilityBase::SyncIdentityIntoAssetTags()
{
	if (AbilityTag.IsValid() && !GetAssetTags().HasTagExact(AbilityTag))
	{
		FGameplayTagContainer Tags = GetAssetTags();
		Tags.AddTag(AbilityTag);
		SetAssetTags(Tags);
	}
	const FName CooldownFName(*(AbilityTag.ToString() + TEXT(".Cooldown")));
	FGameplayTag CooldownTag = FGameplayTag::RequestGameplayTag(CooldownFName);
	if (CooldownTag.IsValid() && !ActivationBlockedTags.HasTagExact(CooldownTag))
	{
		ActivationBlockedTags.AddTag(CooldownTag);
	}
}

void UPWGameplayAbilityBase::AddTagIfMissing(UGameplayTagsSettings* Settings, const FName& TagFName, const FString& Comment)
{
	if (!UGameplayTagsManager::Get().RequestGameplayTag(TagFName, /*ErrorIfNotFound*/false).IsValid())
	{
		FGameplayTagTableRow Row;
		Row.Tag = TagFName;
		Row.DevComment = Comment;
		Settings->GameplayTagList.Add(Row);
	}
}

bool UPWGameplayAbilityBase::EnsureProjectTagExists(const FString& Tag, const FString& Comment)
{
	if (UGameplayTagsSettings* Settings = GetMutableDefault<UGameplayTagsSettings>())
	{
		const FName IdentityFName(*Tag);
		const FName CooldownFName(*(Tag + TEXT(".Cooldown")));

		AddTagIfMissing(Settings, IdentityFName, Comment);
		AddTagIfMissing(Settings, CooldownFName, Comment);
		AddTagIfMissing(Settings, CooldownFName, Comment);

		Settings->SaveConfig();
		Settings->TryUpdateDefaultConfigFile();
		UGameplayTagsManager::Get().EditorRefreshGameplayTagTree();
		return true;
	}
	return false;
}

void UPWGameplayAbilityBase::GenerateIdentityTag()
{
	FString ClassName = GetClass()->GetName();
	ClassName.RemoveFromEnd(TEXT("_C"));
	if (ClassName.StartsWith(TEXT("BP_"))) { ClassName.RightChopInline(3); }

	const FString IdentityStr = FString::Printf(TEXT("Ability.Spell.%s"), *ClassName);
	const FString CooldownStr = IdentityStr + TEXT(".Cooldown");

	EnsureProjectTagExists(IdentityStr, TEXT("Auto-generated by GASCore"));

	const FGameplayTag IdentityTag = UGameplayTagsManager::Get().RequestGameplayTag(*IdentityStr, false);
	const FGameplayTag CooldownTag = UGameplayTagsManager::Get().RequestGameplayTag(*CooldownStr, false);

	Modify();

	if (IdentityTag.IsValid())
	{
		AbilityTag = IdentityTag;
	}
	if (CooldownTag.IsValid() && !ActivationBlockedTags.HasTagExact(CooldownTag))
	{
		ActivationBlockedTags.AddTag(CooldownTag);
	}
	if (!ActivationBlockedTags.HasTagExact(PWTags::Ability::State::Casting))
	{
		ActivationBlockedTags.AddTag(PWTags::Ability::State::Casting);
	}


	SyncIdentityIntoAssetTags();

	PostEditChange();
	MarkPackageDirty();
}

void UPWGameplayAbilityBase::PostLoad()
{
	Super::PostLoad();
	SyncIdentityIntoAssetTags();
}

void UPWGameplayAbilityBase::PostEditChangeProperty(FPropertyChangedEvent& E)
{
	Super::PostEditChangeProperty(E);
	SyncIdentityIntoAssetTags();

	if (E.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UPWGameplayAbilityBase, AbilityModules))
	{
		TMap<UClass*, int32> Counters;
		for (UPWAbilityModule* M : AbilityModules)
		{
			if (!M) continue;
			int32& Count = Counters.FindOrAdd(M->GetClass());
			M->ModuleIndex = Count;
			++Count;
		}
	}
}

#endif
