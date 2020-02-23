// Copyright 2020 - Trifolium Digital Limited

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "PluginPlatformHelpers.h"


/**
 * Slate widget to allow users to edit Config Settings
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


	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	/** Destructor */
	~SPlatformListFilter();

private:
	/**
	 *
	 */
	void OnFilterButtonToggled(EPlatformFilter Filter, bool bWasEnabled);

private:

	//
	TSharedPtr<SHorizontalBox> FilterHBox;

	//
	FOnPluginPlatformFilterChanged OnPluginPlatformFilterChanged;

	//
	EPlatformFilter CurrentFilter = EPlatformFilter::All;

	// 
	TArray<FString> PlatformsExplicitlySupported;
};