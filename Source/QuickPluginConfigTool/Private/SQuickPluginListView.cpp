// Copyright 2020 - Trifolium Digital Limited

#include "SQuickPluginListView.h"
#include "SPlatformFilter.h"
#include "Interfaces/IPluginManager.h"
#include "Interfaces/IProjectManager.h"
#include "SPlatformFilter.h"
#include "Interfaces/ITargetPlatform.h"
#include "Interfaces/ITargetPlatformManagerModule.h"

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

	static FName ListHeader_Dependencies("Dependencies");
	const float ListHeader_Dependencies_Ratio = 3.0f / 5.0f;
}


///////////////////////////////////////////////////////////////////////
// SQuickPluginListView


void SQuickPluginListView::Construct(const FArguments& InArgs)
{
	PopulatePluginsAvailable();
	SortByColumn = PluginListViewHelpers::ListHeader_PluginName;
	ListSortMode = EColumnSortMode::Ascending;

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SPlatformListFilter)
			.OnPluginPlatformFilterChanged(this, &SQuickPluginListView::OnPlatformFilterChanged)
			.PlatformsExplicitlySupported(FoundPlatforms)
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
//					+ SHeaderRow::Column(PluginListViewHelpers::ListHeader_PluginLocation) // We might decide to show this later.
//					.DefaultLabel(LOCTEXT("PluginLocationHeader", "Plugin Location"))
//					.FillWidth(PluginListViewHelpers::ListHeader_PluginLocation_Ratio)
//					.SortMode(this, &SQuickPluginListView::GetColumnSortMode, PluginListViewHelpers::ListHeader_PluginLocation)
//					.OnSort(this, &SQuickPluginListView::OnColumnSortModeChanged)
					+ SHeaderRow::Column(PluginListViewHelpers::ListHeader_Dependencies)
					.DefaultLabel(LOCTEXT("PluginDependencies", "Plugins That Depend On This Plugin"))
					.FillWidth(PluginListViewHelpers::ListHeader_Dependencies_Ratio)
					.SortMode(this, &SQuickPluginListView::GetColumnSortMode, PluginListViewHelpers::ListHeader_Dependencies)
					.OnSort(this, &SQuickPluginListView::OnColumnSortModeChanged)
				)
			]
		]
	];
}


void SQuickPluginListView::NotifyProjectFileWriteStatusChanged(bool bIsReadOnly)
{
	bIsProjectWritable = !bIsReadOnly;
}


SQuickPluginListView::~SQuickPluginListView()
{

}


void SQuickPluginListView::PopulatePluginsAvailable()
{
	// Let's iterate over all registered uplugins and grab the information we need for the table.
	for (const TSharedRef<IPlugin>& Plugin : IPluginManager::Get().GetDiscoveredPlugins())
	{
		TSharedRef<FPluginData> PluginInfo = MakeShareable(new FPluginData(Plugin->GetName()));
		PluginInfo->PluginLocation = Plugin->GetBaseDir();
		PluginInfo->bIsEnabledByDefault = Plugin->IsEnabledByDefault(true);
		PluginInfo->bEnabled = Plugin->IsEnabled();
		PluginInfo->Developer = Plugin->GetDescriptor().CreatedBy;

		for (const FModuleDescriptor& Module : Plugin->GetDescriptor().Modules)
		{
			PluginInfo->bHasEditorOnlyModule |= (Module.Type == EHostType::Editor || Module.Type == EHostType::EditorNoCommandlet || Module.Type == EHostType::EditorAndProgram);
			PluginInfo->bIsEditorOnlyPlugin &= (Module.Type == EHostType::Editor || Module.Type == EHostType::EditorNoCommandlet || Module.Type == EHostType::EditorAndProgram);
		}

		for (const FPluginReferenceDescriptor& Dependency : Plugin->GetDescriptor().Plugins)
		{
			PluginInfo->Dependencies.Add(Dependency.Name);
		}

		PluginInfo->SupportedPlatforms = Plugin->GetDescriptor().SupportedTargetPlatforms;

		// Check the modules for specific platforms.
		if (PluginInfo->SupportedPlatforms.Num() == 0 && !PluginInfo->bIsEditorOnlyPlugin)
		{
			for (const FModuleDescriptor& PluginModule : Plugin->GetDescriptor().Modules)
			{
				if (PluginModule.WhitelistPlatforms.Num() == 0 && !(PluginModule.Type == EHostType::Editor || PluginModule.Type == EHostType::EditorNoCommandlet || PluginModule.Type == EHostType::EditorAndProgram))
				{
					PluginInfo->SupportedPlatforms.Empty();
					break;
				}

				for (const FString& WhitelistedPlatform : PluginModule.WhitelistPlatforms)
				{
					PluginInfo->SupportedPlatforms.AddUnique(WhitelistedPlatform);
				}
			}

		}

		// Create info for all found platforms.
		{
			for (const FString& FoundPlatform : PluginInfo->SupportedPlatforms)
			{
				FoundPlatforms.AddUnique(FoundPlatform);
			}
		}


		AllPlugins.Add(PluginInfo);
		FilteredPlugins.Add(PluginInfo);
	}

	// Now that we have processed all plugins, let's fix-up any thing else.
	for (FPluginDataPtr PluginInfo : AllPlugins)
	{
		// Finally check if the platform does support all.
		if (PluginInfo->SupportedPlatforms.Num() == 0 && !PluginInfo->bIsEditorOnlyPlugin)
		{
			PluginInfo->SupportedPlatforms.Append(FoundPlatforms);
			PluginInfo->SupportedPlatforms.Add(TEXT("Others"));
		}
		PluginInfo->SupportedPlatforms.Sort([](const FString& Lhs, const FString& Rhs) { return PlatformIdStringsToFilterId(Lhs) < PlatformIdStringsToFilterId(Rhs); });

		for (const FString& PluginDependency : PluginInfo->Dependencies)
		{
			FPluginDataPtr* DependsOnPlugin = AllPlugins.FindByPredicate([PluginDependency](FPluginDataPtr PluginIt)
				{
					return PluginIt->PluginName == PluginDependency;
				});

			// May depend on a plugin that was missing from the source.
			if (DependsOnPlugin != nullptr)
			{
				(*DependsOnPlugin)->PluginsThatDependOnThis.Add(PluginInfo->PluginName);
			}
		}
	}

	FilteredPlugins.Sort([](const FPluginDataPtr& A, const FPluginDataPtr& B) { return A->PluginName < B->PluginName; });
}


TSharedRef<ITableRow> SQuickPluginListView::OnGenerateWidgetForList(FPluginDataPtr InItem, const TSharedRef<STableViewBase> &OwnerTable)
{
	return SNew(SPluginInfoRow, OwnerTable)
			.PluginDataItem(InItem)
			.CanEditFlag(&bIsProjectWritable);
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
	else if (SortByColumn == PluginListViewHelpers::ListHeader_Dependencies)
	{
		if (ListSortMode == EColumnSortMode::Ascending)
			FilteredPlugins.Sort([](const FPluginDataPtr& A, const FPluginDataPtr& B) { return A->PluginsThatDependOnThis.Num() < B->PluginsThatDependOnThis.Num(); });
		else if (ListSortMode == EColumnSortMode::Descending)
			FilteredPlugins.Sort([](const FPluginDataPtr& A, const FPluginDataPtr& B) { return A->PluginsThatDependOnThis.Num() > B->PluginsThatDependOnThis.Num(); });
	}

	PluginDetailsView->RequestListRefresh();
}


bool SQuickPluginListView::CanEditPlugins() const
{
	return bIsProjectWritable;
}


void SQuickPluginListView::OnPlatformFilterChanged(EPlatformFilter NewFilter)
{
	FilteredPlugins.Empty();
	if (NewFilter != EPlatformFilter::None)
	{
		for (FPluginDataPtr NextPlugin : AllPlugins)
		{
			if (NextPlugin->SupportedPlatforms.Num() == 0)
			{
				FilteredPlugins.Add(NextPlugin);
			}
			else
			{
				TArray<FString> FilteredPlatformIds = PlatformFilterToPlatformIdStrings(NewFilter);
				for (const FString& FilteredPlatform : FilteredPlatformIds)
				{
					if (NextPlugin->SupportedPlatforms.Contains(FilteredPlatform))
					{
						FilteredPlugins.Add(NextPlugin);
						break;
					}
				}
			}
		}
	}

	// Apply the Editor Only Filter. Special case.
	FilteredPlugins = FilteredPlugins.FilterByPredicate([NewFilter](FPluginDataPtr Plugin)
		{
			return Plugin->bIsEditorOnlyPlugin ? (!!(NewFilter & EPlatformFilter::Editor_Only) && Plugin->bIsEditorOnlyPlugin) : true;
		});

	SortList();
	PluginDetailsView->RequestListRefresh();
}



///////////////////////////////////////////////////////////
// SPluginInfoRow


void SPluginInfoRow::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView)
{
	PluginDataItem = InArgs._PluginDataItem;
	CanEditFlag = InArgs._CanEditFlag;

	SMultiColumnTableRow<FPluginDataPtr>::Construct(FSuperRowType::FArguments().Padding(1.0f), InOwnerTableView);
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
				.IsEnabled(this, &SPluginInfoRow::CanEditPlugin)
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
			FText PlatformsLabel = PluginDataItem->bIsEditorOnlyPlugin ? LOCTEXT("EditorOnlyLabel", "Editor Only") : LOCTEXT("AllPlatformsLabel", "All");
			SupportedPlatformsWidget->AddSlot()
				.Padding(2.0f, 0.0f)
				[
					SNew(SBorder)
					.BorderImage(FEditorStyle::GetBrush("SettingsEditor.CheckoutWarningBorder"))
					.BorderBackgroundColor(PluginDataItem->bIsEditorOnlyPlugin ? PlatformColours::Editor_Only : PlatformColours::Others)
					.ToolTipText(PlatformsLabel)
					.HAlign(EHorizontalAlignment::HAlign_Center)
					[
						SNew(STextBlock)
						.Text(PlatformsLabel)
						.Font(FEditorStyle::GetFontStyle("PropertyWindow.NormalFont"))
						.ColorAndOpacity(!PluginDataItem->bIsEditorOnlyPlugin ? FLinearColor::White : FLinearColor::Black)
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

			if (PluginDataItem->bHasEditorOnlyModule)
			{
				SupportedPlatformsWidget->AddSlot()
					.Padding(2.0f, 0.0f)
					[
						SNew(SBorder)
						.BorderImage(FEditorStyle::GetBrush("SettingsEditor.CheckoutWarningBorder"))
						.BorderBackgroundColor(PlatformColours::Editor_Only)
						.ToolTipText(LOCTEXT("EditorLabel", "Editor"))
						.HAlign(EHorizontalAlignment::HAlign_Center)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("EditorLabel", "Editor"))
							.Font(FEditorStyle::GetFontStyle("PropertyWindow.NormalFont"))
							.ColorAndOpacity(FLinearColor::Black)
						]
					];
			}
		}

		ColumnWidget = SupportedPlatformsWidget;
	}
	else if (InColumnName == PluginListViewHelpers::ListHeader_Dependencies)
	{
		FString DependenciesFormattedStr;
		if (PluginDataItem->PluginsThatDependOnThis.Num() > 0)
		{
			DependenciesFormattedStr = FString::Printf(TEXT("Count: %d  -  ["), PluginDataItem->PluginsThatDependOnThis.Num());
			for (int32 DependencyIdx = 0; DependencyIdx < PluginDataItem->PluginsThatDependOnThis.Num(); DependencyIdx++)
			{
				DependenciesFormattedStr += (PluginDataItem->PluginsThatDependOnThis[DependencyIdx] + (DependencyIdx < PluginDataItem->PluginsThatDependOnThis.Num() - 1 ? TEXT(",  ") : TEXT("")));
			}
			DependenciesFormattedStr += TEXT("]");
		}


		ColumnWidget = SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.Padding(6.0f, 0.0f)
			[
				SNew(STextBlock)
				.Text(FText::FromString(DependenciesFormattedStr))
			.Font(FEditorStyle::GetFontStyle("PropertyWindow.NormalFont"))
			];
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


bool SPluginInfoRow::CanEditPlugin() const
{
	return *CanEditFlag;
}

#undef LOCTEXT_NAMESPACE