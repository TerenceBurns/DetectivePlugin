// Copyright 2020 - Trifolium Digital Limited

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWidget.h"
#include "Widgets/SCompoundWidget.h"

/**
 * Slate widget to allow users to edit Config Settings
 */
class SProjectFileInfo
	: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SProjectFileInfo) {}
		SLATE_DEFAULT_SLOT(FArguments, Content)
	SLATE_END_ARGS()


	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	/** Destructor */
	~SProjectFileInfo();


	// SWidget interface
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	// End of SWidget interface

private:
	/** 
	 *
	 */
	FSlateColor GetInfoBackgroundColour() const;

	/**
	 *
	 */
	int32 HandleNoticeSwitcherWidgetIndex() const;

private:

	//
	bool bIsProjectWritable;

	//
	double LastReadOnlyCheckTime = 0.0;
};