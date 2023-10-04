// Copyright (C) 2023 owoDra

#pragma once

#include "Data/Filter/GSCFilterBase.h"

#include "Misc/TextFilterExpressionEvaluator.h"

#include "GSCFilter_SearchableText.generated.h"


/**
 * Collection タイプのフォーマットの SettingData から SettingData を取得する際に
 * 取得する SettingData を検索可能な文字列を用いてフィルタリングするためのクラス
 */
UCLASS(BlueprintType)
class GSCORE_API UGSCFilter_SearchableText final : public UGSCFilterBase
{
public:
	GENERATED_BODY()
public:
	UGSCFilter_SearchableText();

private:
	//
	// 文字列を検索するのに用いる
	//
	FTextFilterExpressionEvaluator SearchTextEvaluator{ ETextFilterExpressionEvaluatorMode::BasicString };

public:
	//
	// コレクションに対してのみ検索をかけるか
	// 
	// true  : コレクションの名前に対してフィルタリングを行う
	// false : プロパティの名前と説明に対してフィルタリングを行う
	//
	UPROPERTY(Transient)
	bool bSearchForCollectionText{ false };

public:
	/**
	 * 検索に使用する文字列を設定する
	 */
	UFUNCTION(BlueprintCallable, Category = "Filter")
	void SetSearchText(const FString& InSearchText);
	
protected:
	virtual bool FilteringSettingData_Implementation(const UGSCCollectionFormatBase* CollectionFormat, const UGSCData_Setting* TestData) const override;

};
