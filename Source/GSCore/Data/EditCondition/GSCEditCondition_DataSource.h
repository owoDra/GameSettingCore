// Copyright (C) 2023 owoDra

#pragma once

#include "Data/EditCondition/GSCEditConditionBase.h"

#include "Data/Picker/GSCPicker_PropertySource.h"

#include "GSCEditCondition_DataSource.generated.h"


/**
 * エディターで関数の選択を可能にする際に関数を絞り込むための型定義のみ行ったダミーのデリゲートのラッパー
 *
 * Tips:
 *  ここで定義されたデリゲートは主に UGSCEditCondition_DataSource 内でのデータソースの設定に使用される。
 */
UCLASS(Abstract, Const, Transient)
class UGSCPickerTemplate_EditConditionDataSource : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * 設定が編集可能かを取得する関数の型を指定するためのデリゲート
	 */
	DECLARE_DYNAMIC_DELEGATE_RetVal(bool, FEditConditionTemplate);

};


/**
 * 指定したデータソースの値に基づいて設定項目の変更可能条件を判断する
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "Condition ByDataSource"))
class GSCORE_API UGSCEditCondition_DataSource final : public UGSCEditConditionBase
{
public:
	GENERATED_BODY()

protected:
	//
	// 編集可能かの Getter 関数名
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DataSource", meta = (
		GetterFuncTemplate = "/Script/GSCore.GSCPickerTemplate_EditConditionDataSource::EditConditionTemplate__DelegateSignature"))
	FGSCPicker_PropertySource EditConditionSource{ FGSCPicker_PropertySource::Empty };

	//
	// EditConditionSource からの値が false だった場合に項目を非表示にするかどうか
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DataSource")
	bool bHideWhenDisabled{ false };

public:
	virtual FGSCEditableState GatherEditableState_Implementation() override;

};
