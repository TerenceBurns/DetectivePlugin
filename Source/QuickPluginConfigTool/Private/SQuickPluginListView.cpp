// Copyright 2020 - Trifolium Digital Limited

#include "SQuickPluginListView.h"
#include "SPlatformFilter.h"
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

	static FName ListHeader_Developer("Developer");
	const float ListHeader_Developer_Ratio = 1.0f / 5.0f;

	static FName ListHeader_SupportedPlatforms("SupportedPlatforms");
	const float ListHeader_SupportedPlatforms_Ratio = 1.33f / 5.0f;

	static FName ListHeader_PluginLocation("PluginLocation");
	const float ListHeader_PluginLocation_Ratio = 3.0f / 5.0f;

	namespace PlatformColours
	{
		static FColor Windows = FColor(177, 70, 194);
		static FColor Mac = FColor(0, 125, 221);
		static FColor Linux = FColor(0, 0, 0);

		static FColor PS4 = FColor(0, 55, 145);
		static FColor XboxOne = FColor(14, 122, 13);
		static FColor Switch = FColor(230, 0, 18);
		
		static FColor Android = FColor(13, 175, 84);
		static FColor IOS = FColor(0, 120, 215);

		static FColor Lumin = FColor(241, 39, 66);
		static FColor HoloLens = FColor(140, 38, 122);

		static FColor All = FColor(75, 75, 75);
		static FColor Misc = FColor(127, 127, 127);
	}
}

///////////////////////////////////////////////////////////////////////
// SQuickPluginListView

void SQuickPluginListView::Construct(const FArguments& InArgs)
{
	PopulatePluginsAvailable();

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SPlatformListFilter)
		]
		+ SVerticalBox::Slot()
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
					+ SHeaderRow::Column(PluginListViewHelpers::ListHeader_SupportedPlatforms)
					.DefaultLabel(LOCTEXT("PluginSupportedPlatformsHeader", "Supported Platforms"))
					.FillWidth(PluginListViewHelpers::ListHeader_SupportedPlatforms_Ratio)
					.OnSort(this, &SQuickPluginListView::OnColumnSortModeChanged)
					+ SHeaderRow::Column(PluginListViewHelpers::ListHeader_PluginLocation)
					.DefaultLabel(LOCTEXT("PluginLocationHeader", "Plugin Location"))
					.FillWidth(PluginListViewHelpers::ListHeader_PluginLocation_Ratio)
					.SortMode(this, &SQuickPluginListView::GetColumnSortMode, PluginListViewHelpers::ListHeader_PluginLocation)
					.OnSort(this, &SQuickPluginListView::OnColumnSortModeChanged)
				)
			]
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
		PluginInfo->Developer = Plugin->GetDescriptor().CreatedBy;
		PluginInfo->SupportedPlatforms = Plugin->GetDescriptor().SupportedTargetPlatforms;

		for (const FModuleDescriptor& Module : Plugin->GetDescriptor().Modules)
		{
			PluginInfo->bIsEditorOnlyPlugin &= (Module.Type == EHostType::Editor || Module.Type == EHostType::EditorNoCommandlet || Module.Type == EHostType::EditorAndProgram);
		}

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


FColor GetBorderColourForPlatform(const FString& InPlatformName)
{
	if (InPlatformName == TEXT("Win32") || InPlatformName == TEXT("Win64"))
		return PluginListViewHelpers::PlatformColours::Windows;
	else if (InPlatformName == TEXT("Mac"))
		return PluginListViewHelpers::PlatformColours::Mac;
	else if (InPlatformName == TEXT("Linux"))
		return PluginListViewHelpers::PlatformColours::Linux;
	else if (InPlatformName == TEXT("PS4"))
		return PluginListViewHelpers::PlatformColours::PS4;
	else if (InPlatformName == TEXT("XboxOne"))
		return PluginListViewHelpers::PlatformColours::XboxOne;
	else if (InPlatformName == TEXT("Switch"))
		return PluginListViewHelpers::PlatformColours::Switch;
	else if (InPlatformName == TEXT("Android"))
		return PluginListViewHelpers::PlatformColours::Android;
	else if (InPlatformName == TEXT("IOS") || InPlatformName == TEXT("TVOS"))
		return PluginListViewHelpers::PlatformColours::IOS;
	else if (InPlatformName == TEXT("Lumin"))
		return PluginListViewHelpers::PlatformColours::Lumin;
	else if (InPlatformName == TEXT("HoloLens"))
		return PluginListViewHelpers::PlatformColours::HoloLens;
	else
		return PluginListViewHelpers::PlatformColours::Misc;
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
	else if (InColumnName == PluginListViewHelpers::ListHeader_SupportedPlatforms)
	{
		TSharedPtr<SHorizontalBox> SupportedPlatformsWidget = SNew(SHorizontalBox);
		if (PluginDataItem->SupportedPlatforms.Num() == 0)
		{
			FText PlatformsLabel = PluginDataItem->bIsEditorOnlyPlugin ? FText::FromString(TEXT("Editor Only")) : FText::FromString(TEXT("All"));
			SupportedPlatformsWidget->AddSlot()
			.Padding(2.0f, 0.0f)
			[
				SNew(SBorder)
				.BorderImage(FEditorStyle::GetBrush("SettingsEditor.CheckoutWarningBorder"))
				.BorderBackgroundColor(PluginListViewHelpers::PlatformColours::Misc)
				.ToolTipText(PlatformsLabel)
				.HAlign(EHorizontalAlignment::HAlign_Center)
				[
					SNew(STextBlock)
					.Text(PlatformsLabel)
					.Font(FEditorStyle::GetFontStyle("PropertyWindow.BoldFont"))
				]
			];
		}
		else
		{
			for (auto& SupportedPlatformStr : PluginDataItem->SupportedPlatforms)
			{
				FColor BGColour = GetBorderColourForPlatform(SupportedPlatformStr);
				SupportedPlatformsWidget->AddSlot()
				.Padding(1.0f, 0.0f)
				[
					SNew(SBorder)
					.BorderImage(FEditorStyle::GetBrush("SettingsEditor.CheckoutWarningBorder"))
					.BorderBackgroundColor(BGColour)
					.ToolTipText(FText::FromString(SupportedPlatformStr))
					.HAlign(EHorizontalAlignment::HAlign_Center)
					[
						SNew(STextBlock)
						.Text(FText::FromString(SupportedPlatformStr))
						.Font(FEditorStyle::GetFontStyle("PropertyWindow.NormalFont"))
					]
				];
			}
		}

		ColumnWidget = SupportedPlatformsWidget;
	}
	else
	{
		ColumnWidget = SNullWidget::NullWidget;
	}

	return ColumnWidget.ToSharedRef();
}


void SPluginInfoRow::OnPluginEnabledChanged(ECheckBoxState EnabledCheckBoxState)
{
	PluginDataItem->bEnabled = EnabledCheckBoxState == ECheckBoxState::Checked;

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