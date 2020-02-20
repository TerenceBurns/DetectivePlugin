// Copyright 2020 - Trifolium Digital Limited

#include "SProjectFileInfo.h"
#include "ProjectDescriptor.h"
#include "Interfaces/IProjectManager.h"
#include "HAL/FileManager.h"

#define LOCTEXT_NAMESPACE "FQuickPluginConfigToolModule"

/** 
 * 
 */
namespace ProjectFileInfoPanelHelpers
{
	static const FSlateColor CanEditColour = FColor(32, 75, 16);
	static const FSlateColor CanNotEditColour = FColor(75, 32, 16);
}

///////////////////////////////////////////////////////////////////////
// SProjectFileInfo


void SProjectFileInfo::Construct(const FArguments& InArgs)
{
	// To use later.
//	const FProjectDescriptor* UProjectDescriptor = IProjectManager::Get().GetCurrentProject();
//	FString Ext = UProjectDescriptor->GetExtension();

	const FString ProjectFilePath = FPaths::GetProjectFilePath();

	ChildSlot
	[
		SNew(SBorder)
		.Padding(4.0f)
		.BorderImage(FEditorStyle::GetBrush("SettingsEditor.CheckoutWarningBorder"))
		.BorderBackgroundColor(this, &SProjectFileInfo::GetInfoBackgroundColour)
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


SProjectFileInfo::~SProjectFileInfo()
{

}


FSlateColor SProjectFileInfo::GetInfoBackgroundColour() const
{
	const bool bIsProjectWritable = !IFileManager::Get().IsReadOnly(*FPaths::GetProjectFilePath());
	return bIsProjectWritable ? ProjectFileInfoPanelHelpers::CanEditColour : ProjectFileInfoPanelHelpers::CanNotEditColour;
}

#undef LOCTEXT_NAMESPACE