// copyright mkurtt96


#include "PWGA_DetailCustomization.h"

#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "GAS/Abilities/PWGameplayAbilityBase.h"

TSharedRef<IDetailCustomization> FPWGA_DetailCustomization::MakeInstance()
{
	return MakeShareable(new FPWGA_DetailCustomization);
}

void FPWGA_DetailCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	DetailBuilder.GetObjectsBeingCustomized(SelectedObjects);

	IDetailCategoryBuilder& Cat = DetailBuilder.EditCategory("Tags");
	Cat.AddCustomRow(FText::FromString("Generate Ability Tags"))
	.WholeRowContent()
	[
		SNew(SButton)
		.Text(FText::FromString("Generate Ability Tags And Assign Defaults"))
		.OnClicked(FOnClicked::CreateSP(this, &FPWGA_DetailCustomization::OnGenerateTagsClicked))
	];
}

FReply FPWGA_DetailCustomization::OnGenerateTagsClicked()
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