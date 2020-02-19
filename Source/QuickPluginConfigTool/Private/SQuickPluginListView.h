// Copyright 2020 - Trifolium Digital Limited

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "SlateOptMacros.h"

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
	bool bIsEnginePlugin;
	
	//
	FString PluginLocation;
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
	SLATE_END_ARGS()


	/** */
	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView)
	{
		PluginDataItem = InArgs._PluginDataItem;

		SMultiColumnTableRow<FPluginDataPtr>::Construct(FSuperRowType::FArguments().Padding(1.0f), InOwnerTableView);
	}
	
	/** */
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override;


private:
	//
	FPluginDataPtr PluginDataItem;
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

private:
	/** */
	void PopulatePluginsAvailable();

private:
	/** */
	TSharedRef<ITableRow> OnGenerateWidgetForList(FPluginDataPtr InItem, const TSharedRef<STableViewBase>& OwnerTable);

private:
	// Access to the list view of represented plugins.
	TSharedPtr<class SListView<FPluginDataPtr>> PluginDetailsView;

	//
	TArray<FPluginDataPtr> FilteredPlugins;

	//
	TArray<FPluginDataPtr> AllPlugins;
};