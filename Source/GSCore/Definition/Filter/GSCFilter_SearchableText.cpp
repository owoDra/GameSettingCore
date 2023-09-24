// Copyright (C) 2023 owoDra

#include "GSCFilter_SearchableText.h"

#include "Definition/GSCData_Setting.h"
#include "Definition/Format/GSCFormatBase.h"
#include "Definition/Format/GSCFormat_Collection.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSCFilter_SearchableText)


#pragma region TextFilterExpressionContext

class FGSCTextFilterExpressionContext : public ITextFilterExpressionContext
{
public:
	explicit FGSCTextFilterExpressionContext(const FString& InPlainText) : PlainText(InPlainText) {}

	virtual bool TestBasicStringExpression(const FTextFilterString& InValue, const ETextFilterTextComparisonMode InTextComparisonMode) const override
	{
		return TextFilterUtils::TestBasicStringExpression(PlainText, InValue, InTextComparisonMode);
	}

	virtual bool TestComplexExpression(const FName& InKey, const FTextFilterString& InValue, const ETextFilterComparisonOperation InComparisonOperation, const ETextFilterTextComparisonMode InTextComparisonMode) const override
	{
		return false;
	}

private:
	const FString& PlainText;

};

#pragma endregion


#pragma region FilterSearchableText

UGSCFilter_SearchableText::UGSCFilter_SearchableText() 
{
}

void UGSCFilter_SearchableText::SetSearchText(const FString& InSearchText)
{
	SearchTextEvaluator.SetFilterText(FText::FromString(InSearchText));
}

bool UGSCFilter_SearchableText::FilteringSettingData_Implementation(const UGSCCollectionFormatBase* CollectionFormat, const UGSCData_Setting* TestData) const
{
	if (!Super::FilteringSettingData_Implementation(CollectionFormat, TestData))
	{
		return false;
	}

	const auto PlainText{ bSearchForCollectionText ? CollectionFormat->ToString() : TestData->GetFormat()->ToString() };

	if (!PlainText.IsEmpty() && !SearchTextEvaluator.GetFilterText().IsEmpty())
	{
		return SearchTextEvaluator.TestTextFilter(FGSCTextFilterExpressionContext(PlainText));
	}

	return false;
}

#pragma endregion
