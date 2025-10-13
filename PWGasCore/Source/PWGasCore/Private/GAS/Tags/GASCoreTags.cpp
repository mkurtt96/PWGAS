#include "GAS/Tags/GASCoreTags.h"

namespace PWTags::Input // The tag "Input" is used to distinguish ability activators. Each ability should be assigned a unique tag "Input.xxxxx" and this input tag can be used to activate the ability. 
{						// See FPWASC_InputBinding.  Ex. Input.Primary, Input.SpecialAttack, Input.Jump, Input.MoveForward...
	UE_DEFINE_GAMEPLAY_TAG(Root, "Input");
	UE_DEFINE_GAMEPLAY_TAG(GlobalCancel, "Input.Cancel");
}

namespace PWTags::Ability
{
	UE_DEFINE_GAMEPLAY_TAG(Root, "Ability");
	UE_DEFINE_GAMEPLAY_TAG(Cooldown, "Ability.Cooldown"); // Used by dynamic Cooldown GE to set SetByCaller value.
}

namespace PWTags::Ability::Activation
{
	UE_DEFINE_GAMEPLAY_TAG(Root, "Ability.Activation");
	UE_DEFINE_GAMEPLAY_TAG(OnGranted, "Ability.Activation.OnGranted");
}

namespace PWTags::Ability::Deactivation
{
	UE_DEFINE_GAMEPLAY_TAG(Root, "Ability.Deactivation");
	UE_DEFINE_GAMEPLAY_TAG(OnRemoval, "Ability.Deactivation.OnRemoval");
}

namespace PWTags::Ability::Control
{
	UE_DEFINE_GAMEPLAY_TAG(Root, "Ability.Control");
	UE_DEFINE_GAMEPLAY_TAG(Cancelable, "Ability.Control.Cancelable"); // Used with Input.Cancel, this must be assign to a GameplayAbility during it's cancelable state. See FPWASC_InputBinding.
}

namespace PWTags::Ability::State
{
	UE_DEFINE_GAMEPLAY_TAG(Root, "Ability.State");
	UE_DEFINE_GAMEPLAY_TAG(Precasting, "Ability.State.Precasting");
	UE_DEFINE_GAMEPLAY_TAG(Casting, "Ability.State.Casting");
}

namespace PWTags::Ability::Spell // The tag "Ability.Spell" is used to distinguish abilities. Each ability should be assigned a unique tag "Ability.Spell.xxxxx" and this input tag can be used to activate the ability.
								 // Each ability must have a cooldown tag as "Ability.Spell.xxxxx.Cooldown" to handle it's cooldown.
								 // IMPORTANT: These 2 tags can be generated and assigned in GA by the CallInEditor function in blueprint details
{								 // See FPWASC_InputBinding.
	UE_DEFINE_GAMEPLAY_TAG(Root, "Ability.Spell");
}

