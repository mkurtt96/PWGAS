#include "PWGASCoreEditor.h"

#include "PWGA_DetailCustomization.h"

#define LOCTEXT_NAMESPACE "FPWGASCoreEditorModule"

void FPWGASCoreEditorModule::StartupModule()
{
	FPropertyEditorModule& Prop = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	Prop.RegisterCustomClassLayout(TEXT("PWGameplayAbilityBase"),
		FOnGetDetailCustomizationInstance::CreateStatic(&FPWGA_DetailCustomization::MakeInstance));
}

void FPWGASCoreEditorModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FPWGASCoreEditorModule, PWGASCoreEditor)