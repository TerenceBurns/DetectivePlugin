// Copyright 2020 - Trifolium Digital Limited

#include "SQuickPluginListView.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FQuickPluginConfigToolModule"

namespace PluginListViewHelpers
{
	static FName ListHeader_EnablePlugin("EnablePlugin");
	const float ListHeader_EnablePlugin_FixedWidth = 24.0f;
	
	static FName ListHeader_PluginName("PluginName");
	const float ListHeader_PluginName_Ratio = 1.0f / 5.0f;

	static FName ListHeader_EnabledByDefault("EnabledByDefault");
	const float ListHeader_EnabledByDefault_Ratio = 1.0f / 5.0f;

	static FName ListHeader_IsEnginePlugin("IsEnginePlugin");
	const float ListHeader_IsEnginePlugin_Ratio = 1.0f / 5.0f;

	static FName ListHeader_PluginLocation("PluginLocation");
	const float ListHeader_PluginLocation_Ratio = 3.0f / 5.0f;

	static FName ListHeader_Developer("Developer");
	const float ListHeader_Developer_Ratio = 1.0f / 5.0f;
}

///////////////////////////////////////////////////////////////////////
// SQuickPluginListView

void SQuickPluginListView::Construct(const FArguments& InArgs)
{
	PopulatePluginsAvailable();

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
				+ SHeaderRow::Column(PluginListViewHelpers::ListHeader_EnablePlugin)
				.DefaultLabel(LOCTEXT("PluginEnableHeader", ""))
				.DefaultTooltip(LOCTEXT("PluginEnableHeaderToolTip", "Enable for Project"))
				.FixedWidth(PluginListViewHelpers::ListHeader_EnablePlugin_FixedWidth)
				+ SHeaderRow::Column(PluginListViewHelpers::ListHeader_PluginName)
				.DefaultLabel(LOCTEXT("PluginNameHeader", "Plugin Name"))
				.FillWidth(PluginListViewHelpers::ListHeader_PluginName_Ratio)
				+ SHeaderRow::Column(PluginListViewHelpers::ListHeader_EnabledByDefault)
				.DefaultLabel(LOCTEXT("PluginEnabledByDefaultHeader", "Enabled By Default?"))
				.FillWidth(PluginListViewHelpers::ListHeader_EnabledByDefault_Ratio)
				+ SHeaderRow::Column(PluginListViewHelpers::ListHeader_PluginLocation)
				.DefaultLabel(LOCTEXT("PluginLocationHeader", "Plugin Location"))
				.FillWidth(PluginListViewHelpers::ListHeader_PluginLocation_Ratio)
				+ SHeaderRow::Column(PluginListViewHelpers::ListHeader_Developer)
				.DefaultLabel(LOCTEXT("PluginIsEnginePluginHeader", "Developer"))
				.FillWidth(PluginListViewHelpers::ListHeader_Developer_Ratio)
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

void SQuickPluginListView::PopulatePluginsAvailable()
{
	for (const TSharedRef<IPlugin>& Plugin : IPluginManager::Get().GetDiscoveredPlugins())
	{
		TSharedRef<FPluginData> PluginInfo = MakeShareable(new FPluginData(Plugin->GetName()));
		PluginInfo->PluginLocation = Plugin->GetBaseDir();
		PluginInfo->bIsEnabledByDefault = Plugin->IsEnabledByDefault();
		PluginInfo->bEnabled = Plugin->IsEnabled();
		PluginInfo->bIsEnginePlugin = Plugin->GetType() == EPluginType::Engine;
		PluginInfo->Developer = Plugin->GetDescriptor().CreatedBy;

		AllPlugins.Add(PluginInfo);
		FilteredPlugins.Add(PluginInfo);
	}

	FilteredPlugins.Sort([](const FPluginDataPtr& A, const FPluginDataPtr& B) { return A->PluginName < B->PluginName; });
}

TSharedRef<ITableRow> SQuickPluginListView::OnGenerateWidgetForList(FPluginDataPtr InItem, const TSharedRef<STableViewBase> &OwnerTable)
{
	return SNew(SPluginInfoRow, OwnerTable)
			.PluginDataItem(InItem);
}


TSharedRef<SWidget> SPluginInfoRow::GenerateWidgetForColumn(const FName& InColumnName)
{
	TSharedPtr<SWidget> ColumnWidget;
	if (InColumnName == PluginListViewHelpers::ListHeader_PluginName)
	{
		ColumnWidget = SNew(STextBlock)
			.Text(FText::FromString(PluginDataItem->PluginName));
	}
	else if (InColumnName == PluginListViewHelpers::ListHeader_EnabledByDefault)
	{
		ColumnWidget = SNew(SBox)
			.HAlign(EHorizontalAlignment::HAlign_Center)
			[
				SNew(SCheckBox)
				.IsChecked(PluginDataItem->bIsEnabledByDefault ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
				.IsEnabled(false)
			];

	}
	else if (InColumnName == PluginListViewHelpers::ListHeader_EnablePlugin)
	{
		ColumnWidget = SNew(SBox)
			.HAlign(EHorizontalAlignment::HAlign_Center)
			[
				SNew(SCheckBox)
				.IsChecked(PluginDataItem->bEnabled ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
			];
	}
	else if (InColumnName == PluginListViewHelpers::ListHeader_IsEnginePlugin)
	{
		ColumnWidget = SNew(SBox)
			.HAlign(EHorizontalAlignment::HAlign_Center)
			[
				SNew(SCheckBox)
				.IsChecked(PluginDataItem->bIsEnginePlugin ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
				.IsEnabled(false)
			];
	}
	else if (InColumnName == PluginListViewHelpers::ListHeader_PluginLocation)
	{
		ColumnWidget = SNew(STextBlock)
			.Text(FText::FromString(PluginDataItem->PluginLocation));
	}
	else if (InColumnName == PluginListViewHelpers::ListHeader_Developer)
	{
		ColumnWidget = SNew(STextBlock)
			.Text(FText::FromString(PluginDataItem->Developer));
	}
	else
	{
		ColumnWidget = SNullWidget::NullWidget;
	}

	return ColumnWidget.ToSharedRef();
}

#undef LOCTEXT_NAMESPACE