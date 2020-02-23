// Copyright 2020 - Trifolium Digital Limited

#include "SPlatformFilter.h"

#define LOCTEXT_NAMESPACE "FQuickPluginConfigToolModule"


///////////////////////////////////////////////////////////////////////
// SPlatformFilterButton

class SPlatformFilterButton
	: public SCompoundWidget
{
public:
	DECLARE_DELEGATE_TwoParams(FOnFilterToggled, EPlatformFilter, bool);

	SLATE_BEGIN_ARGS(SPlatformFilterButton)
		: _PlatformStyleInfo()
		{}

		SLATE_DEFAULT_SLOT(FArguments, Content)
		SLATE_ARGUMENT(TSharedPtr<FPlatformStyleInfo>, PlatformStyleInfo)

		SLATE_EVENT(FOnFilterToggled, OnFilterToggled)
	SLATE_END_ARGS()


	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	/** Destructor */
	~SPlatformFilterButton() {}


private:

	/** 
	 *
	 */
	void OnCheckStatusChanged(ECheckBoxState EnabledCheckBoxState);

	/**
	 *
	 */
	FSlateColor GetFilterForegroundColor() const;

private:

	//
	TSharedPtr<SCheckBox> FilterCheckItem;

	//
	TSharedPtr<FPlatformStyleInfo> PlatformStyleInfo;

	//
	FOnFilterToggled OnFilterToggled;
};


void SPlatformFilterButton::Construct(const FArguments& InArgs)
{
	PlatformStyleInfo = InArgs._PlatformStyleInfo;
	OnFilterToggled = InArgs._OnFilterToggled;

	ChildSlot
	[
		SAssignNew(FilterCheckItem, SCheckBox)
		.Style(FEditorStyle::Get(), "ContentBrowser.FilterButton")
		.OnCheckStateChanged(this, &SPlatformFilterButton::OnCheckStatusChanged)
		.ForegroundColor(this, &SPlatformFilterButton::GetFilterForegroundColor)
		.IsChecked(ECheckBoxState::Checked)
		[
			SNew(STextBlock)
			.Text(FText::FromString(PlatformStyleInfo->PlatformName))
		]
	];
}


void SPlatformFilterButton::OnCheckStatusChanged(ECheckBoxState EnabledCheckBoxState)
{
	OnFilterToggled.ExecuteIfBound(PlatformStyleInfo->FilterId, EnabledCheckBoxState == ECheckBoxState::Checked);
}


FSlateColor SPlatformFilterButton::GetFilterForegroundColor() const
{
	return FilterCheckItem->IsChecked() ? PlatformStyleInfo->StyleColour : FLinearColor::White;
}


///////////////////////////////////////////////////////////////////////
// SPlatformListFilter


void SPlatformListFilter::Construct(const FArguments& InArgs)
{
	OnPluginPlatformFilterChanged = InArgs._OnPluginPlatformFilterChanged;
	PlatformsExplicitlySupported = InArgs._PlatformsExplicitlySupported;
	
	FilterHBox = SNew(SHorizontalBox);

	for (auto& SupportedPlatform : PlatformStyleInfo::AllStyleInfo)
	{
		FilterHBox->AddSlot()
		.Padding(4.0f)
		.AutoWidth()
		[
			SNew(SPlatformFilterButton)
			.PlatformStyleInfo(SupportedPlatform)
			.OnFilterToggled(this, &SPlatformListFilter::OnFilterButtonToggled)
			
		];
	}

	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		[
			FilterHBox.ToSharedRef()
		]
	];
}


SPlatformListFilter::~SPlatformListFilter()
{

}

void SPlatformListFilter::OnFilterButtonToggled(EPlatformFilter Filter, bool bWasEnabled)
{
	if (bWasEnabled)
		CurrentFilter |= Filter;
	else
		CurrentFilter = CurrentFilter & ~Filter;

	OnPluginPlatformFilterChanged.ExecuteIfBound(CurrentFilter);
}


#undef LOCTEXT_NAMESPACE