// Copyright 2020 - Trifolium Digital Limited

#include "SQuickPluginListView.h"
#include "Interfaces/IPluginManager.h"
#include "Interfaces/IProjectManager.h"

#define LOCTEXT_NAMESPACE "FQuickPluginConfigToolModule"

namespace PluginListViewHelpers
{
	static FName ListHeader_EnablePlugin("EnablePlugin");
	const float ListHeader_EnablePlugin_FixedWidth = 24.0f;
	
	static FName ListHeader_PluginName("PluginName");
	const float ListHeader_PluginName_Ratio = 1.5f / 5.0f;

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
			SAssignNew(PluginDetailsView, SListView<FPluginDataPtr>)
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
				.SortMode(this, &SQuickPluginListView::GetColumnSortMode, PluginListViewHelpers::ListHeader_EnablePlugin)
				.OnSort(this, &SQuickPluginListView::OnColumnSortModeChanged)
				+ SHeaderRow::Column(PluginListViewHelpers::ListHeader_PluginName)
				.DefaultLabel(LOCTEXT("PluginNameHeader", "Plugin Name"))
				.FillWidth(PluginListViewHelpers::ListHeader_PluginName_Ratio)
				.SortMode(this, &SQuickPluginListView::GetColumnSortMode, PluginListViewHelpers::ListHeader_PluginName)
				.OnSort(this, &SQuickPluginListView::OnColumnSortModeChanged)
				+ SHeaderRow::Column(PluginListViewHelpers::ListHeader_Developer)
				.DefaultLabel(LOCTEXT("PluginDeveloperHeader", "Developer"))
				.FillWidth(PluginListViewHelpers::ListHeader_Developer_Ratio)
				.SortMode(this, &SQuickPluginListView::GetColumnSortMode, PluginListViewHelpers::ListHeader_Developer)
				.OnSort(this, &SQuickPluginListView::OnColumnSortModeChanged)
				+ SHeaderRow::Column(PluginListViewHelpers::ListHeader_EnabledByDefault)
				.DefaultLabel(LOCTEXT("PluginEnabledByDefaultHeader", "Enabled By Default?"))
				.FillWidth(PluginListViewHelpers::ListHeader_EnabledByDefault_Ratio)
				.SortMode(this, &SQuickPluginListView::GetColumnSortMode, PluginListViewHelpers::ListHeader_EnabledByDefault)
				.OnSort(this, &SQuickPluginListView::OnColumnSortModeChanged)
				+ SHeaderRow::Column(PluginListViewHelpers::ListHeader_IsEnginePlugin)
				.DefaultLabel(LOCTEXT("PluginIsEnginePluginHeader", "Is Engine Plugin?"))
				.FillWidth(PluginListViewHelpers::ListHeader_IsEnginePlugin_Ratio)
				.SortMode(this, &SQuickPluginListView::GetColumnSortMode, PluginListViewHelpers::ListHeader_IsEnginePlugin)
				.OnSort(this, &SQuickPluginListView::OnColumnSortModeChanged)
				+ SHeaderRow::Column(PluginListViewHelpers::ListHeader_PluginLocation)
				.DefaultLabel(LOCTEXT("PluginLocationHeader", "Plugin Location"))
				.FillWidth(PluginListViewHelpers::ListHeader_PluginLocation_Ratio)
				.SortMode(this, &SQuickPluginListView::GetColumnSortMode, PluginListViewHelpers::ListHeader_PluginLocation)
				.OnSort(this, &SQuickPluginListView::OnColumnSortModeChanged)
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
	if (InColumnName == PluginListViewHelpers::ListHeader_EnablePlugin)
	{
		ColumnWidget = SNew(SBox)
			.HAlign(EHorizontalAlignment::HAlign_Center)
			[
				SNew(SCheckBox)
				.IsChecked(PluginDataItem->bEnabled ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
				.OnCheckStateChanged(this, &SPluginInfoRow::OnPluginEnabledChanged)
			];
	}
	else if (InColumnName == PluginListViewHelpers::ListHeader_PluginName)
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
			.Text(FText::FromString(PluginDataItem->PluginLocation))
			.ColorAndOpacity(FSlateColor::UseSubduedForeground());
	}
	else if (InColumnName == PluginListViewHelpers::ListHeader_Developer)
	{
		ColumnWidget = SNew(STextBlock)
			.Text(FText::FromString(PluginDataItem->Developer))
			.ColorAndOpacity(FSlateColor::UseSubduedForeground());
	}
	else
	{
		ColumnWidget = SNullWidget::NullWidget;
	}

	return ColumnWidget.ToSharedRef();
}


void SPluginInfoRow::OnPluginEnabledChanged(ECheckBoxState EnabledCheckBoxState)
{
	FText FailMessage;
	if (IProjectManager::Get().SetPluginEnabled(PluginDataItem->PluginName, EnabledCheckBoxState == ECheckBoxState::Checked, FailMessage))
	{
		if (IProjectManager::Get().SaveCurrentProjectToDisk(FailMessage))
		{
			// Notify user of save?
		}
		else
		{
			FMessageDialog::Open(EAppMsgType::Ok, FailMessage);
		}
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, FailMessage);
	}
}


EColumnSortMode::Type SQuickPluginListView::GetColumnSortMode(const FName InColumnName) const
{
	return InColumnName == SortByColumn ? ListSortMode : EColumnSortMode::None;
}

void SQuickPluginListView::OnColumnSortModeChanged(const EColumnSortPriority::Type SortPriority, const FName& InColumnName, const EColumnSortMode::Type InSortMode)
{
	SortByColumn = InColumnName;
	ListSortMode = InSortMode;
	SortList();
}

void SQuickPluginListView::SortList()
{
	if (SortByColumn == PluginListViewHelpers::ListHeader_PluginName)
	{
		if (ListSortMode == EColumnSortMode::Ascending)
			FilteredPlugins.Sort([](const FPluginDataPtr& A, const FPluginDataPtr& B) { return A->PluginName < B->PluginName; });
		else
			FilteredPlugins.Sort([](const FPluginDataPtr& A, const FPluginDataPtr& B) { return A->PluginName > B->PluginName; });

	}
	else if (SortByColumn == PluginListViewHelpers::ListHeader_EnabledByDefault)
	{
		if (ListSortMode == EColumnSortMode::Ascending)
			FilteredPlugins.Sort([](const FPluginDataPtr& A, const FPluginDataPtr& B) { return A->bIsEnabledByDefault; });
		else if (ListSortMode == EColumnSortMode::Descending)
			FilteredPlugins.Sort([](const FPluginDataPtr& A, const FPluginDataPtr& B) { return !A->bIsEnabledByDefault; });
	}
	else if (SortByColumn == PluginListViewHelpers::ListHeader_EnablePlugin)
	{
		if (ListSortMode == EColumnSortMode::Ascending)
			FilteredPlugins.Sort([](const FPluginDataPtr& A, const FPluginDataPtr& B) { return A->bEnabled; });
		else if (ListSortMode == EColumnSortMode::Descending)
			FilteredPlugins.Sort([](const FPluginDataPtr& A, const FPluginDataPtr& B) { return !A->bEnabled; });
	}
	else if (SortByColumn == PluginListViewHelpers::ListHeader_IsEnginePlugin)
	{
		if (ListSortMode == EColumnSortMode::Ascending)
			FilteredPlugins.Sort([](const FPluginDataPtr& A, const FPluginDataPtr& B) { return A->bIsEnginePlugin; });
		else if (ListSortMode == EColumnSortMode::Descending)
			FilteredPlugins.Sort([](const FPluginDataPtr& A, const FPluginDataPtr& B) { return !A->bIsEnginePlugin; });
	}
	else if (SortByColumn == PluginListViewHelpers::ListHeader_PluginLocation)
	{
		if (ListSortMode == EColumnSortMode::Ascending)
			FilteredPlugins.Sort([](const FPluginDataPtr& A, const FPluginDataPtr& B) { return A->PluginLocation < B->PluginLocation; });
		else if (ListSortMode == EColumnSortMode::Descending)
			FilteredPlugins.Sort([](const FPluginDataPtr& A, const FPluginDataPtr& B) { return A->PluginLocation > B->PluginLocation; });
	}
	else if (SortByColumn == PluginListViewHelpers::ListHeader_Developer)
	{
		if (ListSortMode == EColumnSortMode::Ascending)
			FilteredPlugins.Sort([](const FPluginDataPtr& A, const FPluginDataPtr& B) { return A->Developer < B->Developer; });
		else if (ListSortMode == EColumnSortMode::Descending)
			FilteredPlugins.Sort([](const FPluginDataPtr& A, const FPluginDataPtr& B) { return A->Developer > B->Developer; });
	}

	PluginDetailsView->RequestListRefresh();
}

#undef LOCTEXT_NAMESPACE