#pragma once
#include "NativeGameplayTags.h"


namespace PWTags::Input
{
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GlobalCancel);
}

namespace PWTags::Ability
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cooldown);
}

namespace PWTags::Ability::Activation
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OnGranted);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OnSpawn);
}

namespace PWTags::Ability::Deactivation
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OnRemoval);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OnDeath);
}

namespace PWTags::Ability::Control
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cancelable);
}

namespace PWTags::Ability::State
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Precasting);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Casting);
}

namespace PWTags::Ability::Spell
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);

	inline FGameplayTag GetCooldownTag(const FGameplayTag& AbilityTag)
	{
		return UGameplayTagsManager::Get().RequestGameplayTag(
			*FString::Printf(TEXT("%s.Cooldown"), *AbilityTag.GetTagName().ToString())
		);
	}

	inline FString GetCooldownSuffix()
	{
		return "Cooldown";
	}
}

