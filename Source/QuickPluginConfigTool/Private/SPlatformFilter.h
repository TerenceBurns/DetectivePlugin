// Copyright 2020 - Trifolium Digital Limited

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWidget.h"
#include "Widgets/SCompoundWidget.h"

/**
 *
 */
enum class EPlatformFilter : uint16
{
	Windows = 1 << 0,
	Mac = 1 << 1,
	PS4 = 1 << 2,
	XboxOne = 1 << 3,
	Switch = 1 << 4,
	Android = 1 << 5,
	IOS = 1 << 6,
	TVOS = 1 << 7,
	Lumin = 1 << 8,
	Others = 1 << 9,

	All = Windows | Mac | PS4 | XboxOne | Switch | Android | IOS | TVOS | Lumin | Others
};

/**
 *
 */
struct FPlatformStyleInfo
{
	FString PlatformName;
	FColor StyleColour;
	EPlatformFilter FilterId;
};


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
};