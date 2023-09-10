// Copyright (C) 2023 owoDra

#pragma once

#include "UObject/Object.h"

#include "Definition/Picker/GSCPicker_DataSource.h"

#include "GSCFormatBase.generated.h"

struct FCachedPropertyPath;


/**
 * エディターで関数の選択を可能にする際に関数を絞り込むための型定義のみ行ったダミーのデリゲートのラッパー
 *  
 * Tips:
 *  ここで定義されたデリゲートは主に UGSCFormatBase 内でのデータソースの設定に使用される。
 */
UCLASS(Abstract, Const, Transient)
class UGSCPickerTemplate_FormatBase : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * 設定へアクセス可能なオブジェクトを取得する関数の型を指定するためのデリゲート
	 */
	DECLARE_DYNAMIC_DELEGATE_RetVal(UObject*, FStaticContextTemplate);

	/**
	 * 設定が変更可能かどうかを取得する関数の型を指定するためのデリゲート
	 */
	DECLARE_DYNAMIC_DELEGATE_RetVal(bool, FEditConditionTemplate);

};


/**
 * 設定項目の定義に使用可能な型を定義するクラス
 * このクラスから派生することで様々なデータ型に対応した設定項目の定義が可能となる
 */
UCLASS(Abstract, DefaultToInstanced, EditInlineNew, NotBlueprintable)
class GSCORE_API UGSCFormatBase : public UObject
{
public:
	GENERATED_BODY()
public:
	UGSCFormatBase() {}

protected:
	//
	// 設定へアクセス可能なオブジェクトを取得する関数の名前
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DataSources",
		meta = (ContextFuncTemplate = "/Script/GSCore.GSCPickerTemplate_FormatBase::StaticContextTemplate__DelegateSignature"))
	FGSCPicker_DataSource StaticContextSource;

	//
	// 設定が変更可能かどうかを取得する関数の名前
	// 
	// Tips:
	//  この項目を設定しない場合は常に変更可能になります
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DataSources",
		meta = (GetterFuncTemplate = "/Script/GSCore.GSCPickerTemplate_FormatBase::EditConditionTemplate__DelegateSignature"))
	FGSCPicker_DataSource EditConditionSource;

};
