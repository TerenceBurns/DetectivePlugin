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
		SLATE_ATTRIBUTE(FPlatformStyleInfo, PlatformStyleInfo)
		
		SLATE_EVENT(FOnFilterToggled, OnFilterToggled)
	SLATE_END_ARGS()


	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs)
	{
		PlatformStyleInfo = InArgs._PlatformStyleInfo;
		ChildSlot
		[
			SAssignNew(FilterCheckItem, SCheckBox)
			.Style(FEditorStyle::Get(), "ContentBrowser.FilterButton")
			.OnCheckStateChanged(this, &SPlatformFilterButton::OnCheckStatusChanged)
			.ForegroundColor(this, &SPlatformFilterButton::GetFilterForegroundColor)
			.IsChecked(ECheckBoxState::Checked)
			[
				SNew(STextBlock)
				.Text(FText::FromString(PlatformStyleInfo.Get().PlatformName))
			]
		];
	}

	/** Destructor */
	~SPlatformFilterButton() {}


private:

	/** 
	 *
	 */
	void OnCheckStatusChanged(ECheckBoxState EnabledCheckBoxState)
	{
		OnFilterToggled.ExecuteIfBound(PlatformStyleInfo.Get().FilterId, EnabledCheckBoxState == ECheckBoxState::Checked);
	}

	/**
	 *
	 */
	FSlateColor GetFilterForegroundColor() const
	{
		return FilterCheckItem->IsChecked() ? PlatformStyleInfo.Get().StyleColour : FLinearColor::White;
	}

private:

	//
	TSharedPtr<SCheckBox> FilterCheckItem;

	//
	TAttribute<FPlatformStyleInfo> PlatformStyleInfo;

	//
	FOnFilterToggled OnFilterToggled;
};


///////////////////////////////////////////////////////////////////////
// SPlatformListFilter


void SPlatformListFilter::Construct(const FArguments& InArgs)
{
	FilterHBox = SNew(SHorizontalBox);

	FPlatformStyleInfo Test;
	Test.PlatformName = TEXT("PlayBox1");

	for (int32 i = 0; i < 5; i++)
	{
		Test.StyleColour = FColor(127, 30*i, 57);
		Test.FilterId = (EPlatformFilter)i;
		FilterHBox->AddSlot()
		.Padding(4.0f)
		.AutoWidth()
		[
			SNew(SPlatformFilterButton)
			.PlatformStyleInfo(Test)
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

}

#undef LOCTEXT_NAMESPACE