// Copyright 2020 - Trifolium Digital Limited

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/SHeaderRow.h"
#include "SlateOptMacros.h"
#include "PluginPlatformHelpers.h"


/**
 * Tangible data we care about for plugins
 */
struct FPluginData
{
	FPluginData(const FString& InName)
	 : PluginName(InName)
	{}

	// The ultimate name of the uplugin
	FString PluginName;

	// Flag which determines whether the plugin is enabled by default for projects.
	bool bIsEnabledByDefault;

	// Flag which shows whether the plugin is currently enabled for the project.
	bool bEnabled;

	// Was the uplugin identified as an Editor only plugin?
	bool bIsEditorOnlyPlugin = true;

	// Does the plugin's module list contain one that was for Editors only. Used to generate target platforms.
	bool bHasEditorOnlyModule = false;
	
	// Where on disk the plugin is stored.
	FString PluginLocation;

	// The listed developer in the uplugin's descriptor.
	FString Developer;

	// Which platforms does the plugin list as targets.
	TArray<FString> SupportedPlatforms;

	// Which other plugins are needed for this one to function?
	TArray<FString> Dependencies;

	// Which plugins depend on this being enabled to function.
	TArray<FString> PluginsThatDependOnThis;
};

typedef TSharedPtr<FPluginData> FPluginDataPtr;


/**
 * Slate implementation for a Plugin entry in the table view.
 */
class SPluginInfoRow : public SMultiColumnTableRow<FPluginDataPtr>
{
public:

	SLATE_BEGIN_ARGS(SPluginInfoRow) { }
		SLATE_ARGUMENT(FPluginDataPtr, PluginDataItem)
		SLATE_ARGUMENT(bool*, CanEditFlag)
	SLATE_END_ARGS()


	/** Slate widget construction of the row. */
	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView);
	
	/** The content for the slate widget row. */
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override;

private:
	/** Handle a plugin being enabled/disabled from the tool. */
	void OnPluginEnabledChanged(ECheckBoxState EnabledCheckBoxState);

	/** Determine whether the plugin row can be edited at the given time. */
	bool CanEditPlugin() const;

private:
	// A reference to the Plugin Information this row holds.
	FPluginDataPtr PluginDataItem;

	// Reference to an owning object's flag which is used to determine whether this row can be edited.
	bool* CanEditFlag;
};


/**
 * Slate widget to allow users to view ue4 plugin settings
 */
class SQuickPluginListView
	: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SQuickPluginListView) {}
		SLATE_DEFAULT_SLOT(FArguments, Content)
	SLATE_END_ARGS()


	/** Slate widget which shows a table of plugin data. */
	void Construct(const FArguments& InArgs);

	/**  */
	virtual ~SQuickPluginListView();

public:

	/** Hook whjich allows us to determine whether we can edit the plugins that are used. */
	void NotifyProjectFileWriteStatusChanged(bool bIsReadOnly);

	/** Check whether we changes to plugins should be allowed. */
	bool CanEditPlugins() const;

private:
	/** Determine what plugins that we have available in our engine install. */
	void PopulatePluginsAvailable();

	/** Basic functionality which allows us to sort the table. Handy for finding all enabled plugins etc. */
	void SortList();

private:
	/** For the given plugin information, generate a row in our table. */
	TSharedRef<ITableRow> OnGenerateWidgetForList(FPluginDataPtr InItem, const TSharedRef<STableViewBase>& OwnerTable);

	/** Determine how we are sorting our table. */
	EColumnSortMode::Type GetColumnSortMode(const FName InColumnName) const;

	/** Hook which listens for sort changes from the table widget. */
	void OnColumnSortModeChanged(const EColumnSortPriority::Type SortPriority, const FName& InColumnName, const EColumnSortMode::Type InSortMode);

	/** Listener for changes made to the platform filter in the tool. */
	void OnPlatformFilterChanged(EPlatformFilter NewFilter);

private:
	// Access to the list view of represented plugins.
	TSharedPtr<class SListView<FPluginDataPtr>> PluginDetailsView;

	// The plugins which are currently shown in the list view.
	TArray<FPluginDataPtr> FilteredPlugins;

	// All plugins which could potentaially be shown on the list, dependant on filters.
	TArray<FPluginDataPtr> AllPlugins;

	// How is the view sorted currently (Ascending etc.)?
	EColumnSortMode::Type ListSortMode;

	// Which column is the table view sorted by at this time?
	FName SortByColumn;

	// The list of all platforms which were ascertained from the installed plugins.
	TArray<FString> FoundPlatforms;

	// Is the project's uproject file writable? If not, we cannot make changes to which are enabled.
	bool bIsProjectWritable;
};