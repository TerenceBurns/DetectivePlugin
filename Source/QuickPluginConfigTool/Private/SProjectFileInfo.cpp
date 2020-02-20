// Copyright 2020 - Trifolium Digital Limited

#include "SProjectFileInfo.h"
#include "ProjectDescriptor.h"
#include "Interfaces/IProjectManager.h"

#define LOCTEXT_NAMESPACE "FQuickPluginConfigToolModule"


///////////////////////////////////////////////////////////////////////
// SQuickPluginConfigTool

#pragma optimize("", off)

void SProjectFileInfo::Construct(const FArguments& InArgs)
{
	// To use later.
//	const FProjectDescriptor* UProjectDescriptor = IProjectManager::Get().GetCurrentProject();
//	FString Ext = UProjectDescriptor->GetExtension();

	FString ProjectFilePath = FPaths::GetProjectFilePath();

	// Green for now. We will update so it reflects writable status.
	FColor StatusBGColour(32, 75, 16);

	ChildSlot
	[
		SNew(SBorder)
		.Padding(4.0f)
		.BorderImage(FEditorStyle::GetBrush("SettingsEditor.CheckoutWarningBorder"))
		.BorderBackgroundColor(StatusBGColour)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding( 0.0f, 0.0f, 4.0f, 0.0f )
			[
				SNew(SBox)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Top)
				[
					SNew(SImage)
					.Image(FEditorStyle::GetBrush("LevelEditor.Tabs.Details"))
				]
			]
			+SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("ProjectFileInfoStatusStr", "Status:"))
					.Font(FEditorStyle::GetFontStyle("PropertyWindow.NormalFont"))
				]
				+ SVerticalBox::Slot()
				[
					SNew(STextBlock)
					.Text(FText::FromString(FString::Printf(TEXT("    %s [%s]"), *ProjectFilePath, TEXT("WRITABLE"))))
					.Font(FEditorStyle::GetFontStyle("PropertyWindow.NormalFont"))
				]
			]
		]
	];
}
#pragma optimize("", on)


SProjectFileInfo::~SProjectFileInfo()
{

}

#undef LOCTEXT_NAMESPACE