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


	/** Slate widget which shows information about the project's .uproject file. */
	void Construct(const FArguments& InArgs);

	/**  */
	virtual ~SProjectFileInfo() {}


private:

	// The abs path to the .uproject file of this project.
	FString ProjectFilePath;
};