// Copyright 2020 - Trifolium Digital Limited

#include "SQuickPluginConfigTool.h"
#include "SProjectFileInfo.h"
#include "SQuickPluginListView.h"

#define LOCTEXT_NAMESPACE "FQuickPluginConfigToolModule"


namespace QuickPluginConfigToolViewConstants
{
	const float DefaultSelectionPanelRatio = 0.5f;
	const float DefaultDetailEditorRatio = 1.0f - DefaultSelectionPanelRatio;
}


///////////////////////////////////////////////////////////////////////
// SQuickPluginConfigTool

void SQuickPluginConfigTool::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
		.Padding(4.0f)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			[
				SAssignNew(PluginDetailsView, SQuickPluginListView)			
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SProjectFileInfo)
			]
		]
	];
}


SQuickPluginConfigTool::~SQuickPluginConfigTool()
{

}

#undef LOCTEXT_NAMESPACE