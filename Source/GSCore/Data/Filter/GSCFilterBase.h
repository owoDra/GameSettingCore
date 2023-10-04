// Copyright (C) 2023 owoDra

#pragma once

#include "UObject/Object.h"

#include "GSCFilterBase.generated.h"

class UGSCData_Setting;


/**
 * Collection タイプのフォーマットの SettingData から SettingData を取得する際に
 * 取得する SettingData をフィルタリングするためのクラス
 */
UCLASS(Abstract, Blueprintable)
class GSCORE_API UGSCFilterBase : public UObject
{
public:
	GENERATED_BODY()
public:

	friend class UGSCCollectionFormatBase;

public:
	//
	// IsDeprecated() で true を返す SettingData を許可するか
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Filter")
	bool bAllowDeprecated{ false };

protected:
	/**
	 * SettingData をフィルタリングし、フィルターを通過した場合はリストに追加する
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Filter")
	bool FilteringSettingData(const UGSCCollectionFormatBase* CollectionFormat, const UGSCData_Setting* TestData) const;
	virtual bool FilteringSettingData_Implementation(const UGSCCollectionFormatBase* CollectionFormat, const UGSCData_Setting* TestData) const;

};
