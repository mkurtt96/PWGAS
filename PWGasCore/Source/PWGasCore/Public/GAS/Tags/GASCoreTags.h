#pragma once
#include "NativeGameplayTags.h"


namespace PWTags::Input
{
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GlobalCancel);
}

namespace PWTags::ASC
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
}

namespace PWTags::ASC::State
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Casting);
}

namespace PWTags::MontageEvent
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Trigger);
}

namespace PWTags::Ability
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
}

namespace PWTags::Ability::SetByCaller
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cooldown);
}

namespace PWTags::Ability::Activation
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OnGranted);
}

namespace PWTags::Ability::Deactivation
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OnRemoval);
}

namespace PWTags::Ability::Control
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cancelable);
}

namespace PWTags::Ability::Skill
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

