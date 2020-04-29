// Copyright 2020 - Trifolium Digital Limited

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "PluginPlatformHelpers.h"


/**
 * Slate widget used to control the list view based on toggled platforms.
 */
class SPlatformListFilter
	: public SCompoundWidget
{
public:
	DECLARE_DELEGATE_OneParam(FOnPluginPlatformFilterChanged, EPlatformFilter);

	SLATE_BEGIN_ARGS(SPlatformListFilter) {}
		SLATE_DEFAULT_SLOT(FArguments, Content)
		SLATE_EVENT(FOnPluginPlatformFilterChanged, OnPluginPlatformFilterChanged)
		
		SLATE_ARGUMENT(TArray<FString>, PlatformsExplicitlySupported)
	SLATE_END_ARGS()


	/** Create a slate widget which lists all the platforms we can toggle in the view. */
	void Construct(const FArguments& InArgs);

	/** Destructor */
	virtual ~SPlatformListFilter();

private:
	/**
	 * Handle a platform being updated. We should notify the list view.
	 */
	void OnFilterButtonToggled(EPlatformFilter Filter, bool bWasEnabled);

private:

	// The widget which holds the platform toggle buttons.
	TSharedPtr<SHorizontalBox> FilterHBox;

	// Hook which can be registered with to hear about platform changes in the filter.
	FOnPluginPlatformFilterChanged OnPluginPlatformFilterChanged;

	// What is the current filter? (Default to show all platforms)
	EPlatformFilter CurrentFilter = EPlatformFilter::All;

	// Which platforms were found by the plugin system when parsing uplugin files.
	TArray<FString> PlatformsExplicitlySupported;
};