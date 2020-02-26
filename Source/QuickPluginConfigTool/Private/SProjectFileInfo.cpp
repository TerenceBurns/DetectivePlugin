// Copyright 2020 - Trifolium Digital Limited

#include "SProjectFileInfo.h"
#include "ProjectDescriptor.h"
#include "Widgets/Layout/SWidgetSwitcher.h"
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
	ProjectFilePath = FPaths::GetProjectFilePath();
	bIsProjectWritable = !IFileManager::Get().IsReadOnly(*ProjectFilePath);
	
	CreateProjectWritableNoticeWidget();

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		[
			ProjectWritableNotice.ToSharedRef()
		]
	];
}


SProjectFileInfo::~SProjectFileInfo()
{

}


FSlateColor SProjectFileInfo::GetInfoBackgroundColour() const
{
	return bIsProjectWritable ? ProjectFileInfoPanelHelpers::CanEditColour : ProjectFileInfoPanelHelpers::CanNotEditColour;
}


// Callback for getting the widget index for the notice switcher.
int32 SProjectFileInfo::HandleNoticeSwitcherWidgetIndex() const
{
	return bIsProjectWritable ? 1 : 0; // Unlocked by default, For now
}

void SProjectFileInfo::CreateProjectWritableNoticeWidget()
{
	SAssignNew(ProjectWritableNotice, SBorder)
	.Padding(4.0f)
	.BorderImage(FEditorStyle::GetBrush("SettingsEditor.CheckoutWarningBorder"))
	.BorderBackgroundColor(this, &SProjectFileInfo::GetInfoBackgroundColour)
	[
		SNew(SWidgetSwitcher)
		.WidgetIndex(this, &SProjectFileInfo::HandleNoticeSwitcherWidgetIndex)
		// Locked slot
		+ SWidgetSwitcher::Slot()
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
					.Text(FText::FromString(FString::Printf(TEXT("    %s [%s]"), *ProjectFilePath, TEXT("LOCKED"))))
					.Font(FEditorStyle::GetFontStyle("PropertyWindow.NormalFont"))
				]
			]
		]

		// Unlocked slot
		+ SWidgetSwitcher::Slot()
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


void SProjectFileInfo::NotifyProjectFileWriteStatusChanged(bool bIsReadOnly)
{
	bIsProjectWritable = !bIsReadOnly;
}

#undef LOCTEXT_NAMESPACE