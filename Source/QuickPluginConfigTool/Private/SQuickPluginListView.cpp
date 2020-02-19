// Copyright 2020 - Trifolium Digital Limited

#include "SQuickPluginListView.h"

#define LOCTEXT_NAMESPACE "FQuickPluginConfigToolModule"

namespace PluginListViewHelpers
{
	static FName ListHeader_PluginName("PluginName");
	const float ListHeader_PluginName_Ratio = 0.25f;

	static FName ListHeader_EnabledByDefault("EnabledByDefault");
	const float ListHeader_EnabledByDefault_Ratio = 0.25f;

	static FName ListHeader_IsEnginePlugin("IsEnginePlugin");
	const float ListHeader_IsEnginePlugin_Ratio = 0.5f;
}

///////////////////////////////////////////////////////////////////////
// SQuickPluginListView

void SQuickPluginListView::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
		.Padding(4.0f)
		[	
			SNew(SListView<FPluginDataPtr>)
			.ItemHeight(24)						
			.ScrollbarVisibility(EVisibility::Visible)
			.ListItemsSource(&FilteredPlugins)
			.OnGenerateRow(this, &SQuickPluginListView::OnGenerateWidgetForList)
			.HeaderRow
			(
				SNew(SHeaderRow)
				+ SHeaderRow::Column(PluginListViewHelpers::ListHeader_PluginName)
				.DefaultLabel(LOCTEXT("PluginNameHeader", "Plugin Name"))
				.FillWidth(PluginListViewHelpers::ListHeader_PluginName_Ratio)
				+ SHeaderRow::Column(PluginListViewHelpers::ListHeader_EnabledByDefault)
				.DefaultLabel(LOCTEXT("PluginEnabledByDefaultHeader", "Enabled By Default?"))
				.FillWidth(PluginListViewHelpers::ListHeader_EnabledByDefault_Ratio)
				+ SHeaderRow::Column(PluginListViewHelpers::ListHeader_IsEnginePlugin)
				.DefaultLabel(LOCTEXT("PluginIsEnginePluginHeader", "Is Engine Plugin?"))
				.FillWidth(PluginListViewHelpers::ListHeader_IsEnginePlugin_Ratio)
			)
		]
	];
}


SQuickPluginListView::~SQuickPluginListView()
{

}

TSharedRef<ITableRow> SQuickPluginListView::OnGenerateWidgetForList(FPluginDataPtr InItem, const TSharedRef<STableViewBase> &OwnerTable)
{
	return SNew(SPluginInfoRow, OwnerTable)
			.PluginDataItem(InItem);
}


TSharedRef<SWidget> SPluginInfoRow::GenerateWidgetForColumn(const FName& InColumnName)
{
	if (InColumnName == PluginListViewHelpers::ListHeader_PluginName)
	{

	}
	if (InColumnName == PluginListViewHelpers::ListHeader_EnabledByDefault)
	{

	}
	if (InColumnName == PluginListViewHelpers::ListHeader_IsEnginePlugin)
	{

	}
	return SNullWidget::NullWidget;
}

#undef LOCTEXT_NAMESPACE