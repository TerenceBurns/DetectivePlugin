// Copyright 2020 - Trifolium Digital Limited

#include "SQuickPluginConfigTool.h"
#include "SProjectFileInfo.h"
#include "SQuickPluginListView.h"

#define LOCTEXT_NAMESPACE "FQuickPluginConfigToolModule"


namespace QuickPluginConfigToolViewConstants
{
	const double ProjectReadOnlyFlagPeriodicCheckTime = 1.0f;
}


///////////////////////////////////////////////////////////////////////
// SQuickPluginConfigTool

void SQuickPluginConfigTool::Construct(const FArguments& InArgs)
{
	bLastProjectCheckWasReadOnly = IFileManager::Get().IsReadOnly(*FPaths::GetProjectFilePath());

	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
		.Padding(4.0f)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SAssignNew(ProjectFileInfo, SProjectFileInfo)
				.Visibility_Lambda([this]() { return bLastProjectCheckWasReadOnly ? EVisibility::Visible : EVisibility::Collapsed; })
			]
			+ SVerticalBox::Slot()
			[
				SAssignNew(PluginDetailsView, SQuickPluginListView)			
			]
		]
	];

	PluginDetailsView->NotifyProjectFileWriteStatusChanged(bLastProjectCheckWasReadOnly);
}


SQuickPluginConfigTool::~SQuickPluginConfigTool()
{

}


void SQuickPluginConfigTool::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	if (InCurrentTime - LastReadOnlyCheckTime >= QuickPluginConfigToolViewConstants::ProjectReadOnlyFlagPeriodicCheckTime)
	{
		if (IFileManager::Get().IsReadOnly(*FPaths::GetProjectFilePath()) != bLastProjectCheckWasReadOnly)
		{
			bLastProjectCheckWasReadOnly = !bLastProjectCheckWasReadOnly;
			PluginDetailsView->NotifyProjectFileWriteStatusChanged(bLastProjectCheckWasReadOnly);
		}
		LastReadOnlyCheckTime = InCurrentTime;
	}
}

#undef LOCTEXT_NAMESPACE