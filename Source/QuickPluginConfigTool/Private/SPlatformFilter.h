// Copyright 2020 - Trifolium Digital Limited

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWidget.h"
#include "Widgets/SCompoundWidget.h"

/**
 *
 */
struct FPlatformStyleInfo
{
	FString PlatformName;
	FColor StyleColour;
};


/**
 * Slate widget to allow users to edit Config Settings
 */
class SPlatformListFilter
	: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SPlatformListFilter) {}
		SLATE_DEFAULT_SLOT(FArguments, Content)
	SLATE_END_ARGS()


	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	/** Destructor */
	~SPlatformListFilter();

private:

	//
	TSharedPtr<SHorizontalBox> FilterHBox;
};