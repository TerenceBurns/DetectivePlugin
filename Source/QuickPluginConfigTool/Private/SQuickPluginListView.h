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
 *
 */
struct FPluginData
{
	FPluginData(const FString& InName)
	 : PluginName(InName)
	{}

	//
	FString PluginName;

	//
	bool bIsEnabledByDefault;

	//
	bool bEnabled;

	//
	bool bIsEditorOnlyPlugin = true;
	
	//
	FString PluginLocation;

	//
	FString Developer;

	// 
	TArray<FString> SupportedPlatforms;

	//
	TArray<FString> Dependencies;

	//
	TArray<FString> PluginsThatDependOnThis;
};

typedef TSharedPtr<FPluginData> FPluginDataPtr;


/**
 *
 */
class SPluginInfoRow : public SMultiColumnTableRow<FPluginDataPtr>
{
public:

	SLATE_BEGIN_ARGS(SPluginInfoRow) { }
		SLATE_ARGUMENT(FPluginDataPtr, PluginDataItem)
		SLATE_ARGUMENT(bool*, CanEditFlag)
	SLATE_END_ARGS()


	/** */
	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView);
	
	/** */
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override;

private:
	/** */
	void OnPluginEnabledChanged(ECheckBoxState EnabledCheckBoxState);

	/** */
	bool CanEditPlugin() const;

private:
	//
	FPluginDataPtr PluginDataItem;

	// 
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


	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	/** Destructor */
	~SQuickPluginListView();

public:

	/** */
	void NotifyProjectFileWriteStatusChanged(bool bIsReadOnly);

	/** */
	bool CanEditPlugins() const;

private:
	/** */
	void PopulatePluginsAvailable();

	/** */
	void SortList();

private:
	/** */
	TSharedRef<ITableRow> OnGenerateWidgetForList(FPluginDataPtr InItem, const TSharedRef<STableViewBase>& OwnerTable);

	/** */
	EColumnSortMode::Type GetColumnSortMode(const FName InColumnName) const;

	/** */
	void OnColumnSortModeChanged(const EColumnSortPriority::Type SortPriority, const FName& InColumnName, const EColumnSortMode::Type InSortMode);

	/** */
	void OnPlatformFilterChanged(EPlatformFilter NewFilter);

private:
	// Access to the list view of represented plugins.
	TSharedPtr<class SListView<FPluginDataPtr>> PluginDetailsView;

	//
	TArray<FPluginDataPtr> FilteredPlugins;

	//
	TArray<FPluginDataPtr> AllPlugins;

	// 
	EColumnSortMode::Type ListSortMode;

	//
	FName SortByColumn;

	//
	TArray<FString> FoundPlatforms;

	//
	bool bIsProjectWritable;
};