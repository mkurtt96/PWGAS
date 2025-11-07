// copyright mkurtt96


#include "GAS/Abilities/PWGameplayAbilityBase.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagsManager.h"
#include "GameplayTagsSettings.h"
#include "GameFramework/PlayerState.h"
#include "GAS/Abilities/Modules/ActionModules/PWAbilityInstantEffectModule.h"
#include "GAS/Abilities/Modules/ActionModules/PWAbilityMultiActorModule.h"
#include "GAS/Abilities/Modules/ActionModules/PWActionModule.h"
#include "GAS/Abilities/Modules/ControlModules/PWAbilityTargetingModule.h"
#include "GAS/Abilities/Modules/DataModules/PWAbilityRadiusModule.h"
#include "PWGasCore/Public/GAS/Abilities/Modules/PWAbilityModule.h"
#include "GAS/ASC/PWASC_InputBinding.h"
#include "GAS/Tags/GASCoreTags.h"
#include "Targeting/Data/PWTargetingData.h"
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

	ForEachModule([this](UPWAbilityModule* Mod)
	{
		Mod->Initialize(this);
		Mod->OnAbilityActivated();
	});
}

USpellParams* UPWGameplayAbilityBase::MakeSpellParams()
{
	USpellParams* Params = NewObject<USpellParams>(this);
	Params->SourceAvatar = GetAvatarActorFromActorInfo();
	Params->AbilityLevel = GetAbilityLevel();

	if (const APlayerState* PS = GetPawn() ? GetPawn()->GetPlayerState() : nullptr)
		Params->SourcePlayerUniqueId = PS->GetUniqueId();

	if (!Params->More)
		Params->More = NewObject<UMultiDataArray>(Params);

	return Params;
}

FPWTargetingResult UPWGameplayAbilityBase::ComputeTargetOnce() const
{
	if (UPWAbilityTargetingModule* TMod = GetControlModule<UPWAbilityTargetingModule>())
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

void UPWGameplayAbilityBase::GetAnimMontageFromActor(UAnimMontage*& OutMontage, float& OutAnimRate, int Index) const
{
	const AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor || !AnimationTags.IsValidIndex(Index) || !AnimationTags[Index].IsValid())
		return;

	if (AvatarActor->GetClass()->ImplementsInterface(UPWAnimSetProvider::StaticClass()))
	{
		OutMontage = IPWAnimSetProvider::Execute_GetMontageForTag(AvatarActor, AnimationTags[Index], OutAnimRate);
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("AvatarActor: %s does not use the IPWAnimSetProvider, See UPWGameplayAbilityBase::GetAnimationMontageFromActor"), *AvatarActor->GetName())
}

FGameplayTagContainer UPWGameplayAbilityBase::GetCooldownGameplayTags() const
{
	FGameplayTagContainer MutableTags = FGameplayTagContainer();
	MutableTags.Reset();

	if (const FGameplayTagContainer* Parent = Super::GetCooldownTags())
		MutableTags.AppendTags(*Parent);

	if (AbilityTag.IsValid())
	{
		const FGameplayTag CooldownTag = PWTags::Ability::Skill::GetCooldownTag(AbilityTag);
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
		const FGameplayTag CooldownTag = PWTags::Ability::Skill::GetCooldownTag(AbilityTag);
		SpecHandle.Data.Get()->DynamicGrantedTags.AppendTags(CooldownTag.GetSingleTagContainer());
		SpecHandle.Data.Get()->SetSetByCallerMagnitude(PWTags::Ability::SetByCaller::Cooldown, Cooldown.GetValueAtLevel(GetAbilityLevel()));
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
	ForEachModule([this](UPWAbilityModule* Mod)
	{
		Mod->OnAbilityEnded();
	});
}

void UPWGameplayAbilityBase::EnsureRequiredDataModules()
{
	TArray<TSubclassOf<UPWDataModule>> Required;
	ForEachModule([&Required](UPWAbilityModule* Module)
	{
		Module->GetRequiredDataModules(Required);
	});

	for (TSubclassOf<UPWDataModule> DataClass : Required)
	{
		if (!DataClass)
			continue;

		bool bExists = false;
		for (UPWDataModule* M : DataModules)
			if (M && M->IsA(DataClass))
			{
				bExists = true;
				break;
			}

		if (!bExists)
		{
			UPWDataModule* NewData = NewObject<UPWDataModule>(this, DataClass, NAME_None, RF_Transactional);
			DataModules.Add(NewData);
			NewData->Initialize(this);
			UE_LOG(LogTemp, Log, TEXT("[PWGASCore] Auto-added required DataModule: %s"), *DataClass->GetName());
		}
	}
}

void UPWGameplayAbilityBase::ForEachModule(TFunctionRef<void(UPWAbilityModule*)> Callback)
{
	if (ActionModule)
		Callback(ActionModule);

	for (UPWDataModule* M : DataModules)
		if (M)
			Callback(M);

	for (UPWControlModule* M : ControlModules)
		if (M)
			Callback(M);
}

UPWAbilityInstantEffectModule* UPWGameplayAbilityBase::GetInstantEffectModule() const
{
	return GetActionModule<UPWAbilityInstantEffectModule>();
}

UPWAbilityActorModule* UPWGameplayAbilityBase::GetActorModule() const
{
	return GetActionModule<UPWAbilityActorModule>();
}

UPWAbilityMultiActorModule* UPWGameplayAbilityBase::GetMultiActorModule() const
{
	return GetActionModule<UPWAbilityMultiActorModule>();
}

UPWAbilityProjectileModule* UPWGameplayAbilityBase::GetProjectileModule() const
{
	return GetActionModule<UPWAbilityProjectileModule>();
}

UPWAbilityMultiProjectileModule* UPWGameplayAbilityBase::GetMultiProjectileModule() const
{
	return GetActionModule<UPWAbilityMultiProjectileModule>();
}

UPWAbilityPrecastModule* UPWGameplayAbilityBase::GetPrecastModule() const
{
	return GetControlModule<UPWAbilityPrecastModule>();
}

UPWAbilityEffectModule* UPWGameplayAbilityBase::GetEffectModule() const
{
	return GetDataModule<UPWAbilityEffectModule>();
}

UPWAbilityAuraModule* UPWGameplayAbilityBase::GetAuraModule() const
{
	return GetActionModule<UPWAbilityAuraModule>();
}

UPWAbilityTargetingModule* UPWGameplayAbilityBase::GetTargetingModule() const
{
	return GetControlModule<UPWAbilityTargetingModule>();
}

UPWAbilityRangeModule* UPWGameplayAbilityBase::GetRangeModule() const
{
	return GetDataModule<UPWAbilityRangeModule>();
}

UPWAbilityRadiusModule* UPWGameplayAbilityBase::GetRadiusModule() const
{
	return GetDataModule<UPWAbilityRadiusModule>();
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
	if (ClassName.StartsWith(TEXT("GA_"))) { ClassName.RightChopInline(3); }

	const FString IdentityStr = FString::Printf(TEXT("Ability.Skill.%s"), *ClassName);
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
	EnsureRequiredDataModules();
}

#endif
