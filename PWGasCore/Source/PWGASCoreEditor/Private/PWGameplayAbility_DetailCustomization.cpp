// copyright mkurtt96


#include "PWGameplayAbility_DetailCustomization.h"

#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "GAS/Abilities/PWGameplayAbilityBase.h"
#include "GAS/Abilities/PWModularGameplayAbility.h"

TSharedRef<IDetailCustomization> FPWGameplayAbility_DetailCustomization::MakeInstance()
{
	return MakeShareable(new FPWGameplayAbility_DetailCustomization);
}

void FPWGameplayAbility_DetailCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	DetailBuilder.GetObjectsBeingCustomized(SelectedObjects);

	IDetailCategoryBuilder& Cat = DetailBuilder.EditCategory("Tags");
	Cat.AddCustomRow(FText::FromString("Generate Ability Tags"))
	.WholeRowContent()
	[
		SNew(SButton)
		.Text(FText::FromString("Generate Ability Tags And Assign Defaults"))
		.OnClicked(FOnClicked::CreateSP(this, &FPWGameplayAbility_DetailCustomization::OnGenerateTagsClicked))
	];
}

FReply FPWGameplayAbility_DetailCustomization::OnGenerateTagsClicked()
{
	for (TWeakObjectPtr<UObject> Obj : SelectedObjects)
	{
		if (UPWGameplayAbilityBase* GA = Cast<UPWGameplayAbilityBase>(Obj.Get()))
		{
			GA->GenerateIdentityTag();
		}
	}
	return FReply::Handled();
}