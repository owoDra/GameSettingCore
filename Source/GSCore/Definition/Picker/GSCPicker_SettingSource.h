// Copyright (C) 2023 owoDra

#pragma once

#include "UObject/Object.h"

#include "GSCPicker_SettingSource.generated.h"


/**
 * エディターで関数の選択を可能にする際に関数を絞り込むための型定義のみ行ったダミーのデリゲートのラッパー
 *
 * Tips:
 *  ここで定義されたデリゲートは主に FGSCPicker_SettingSource 内でのデータソースの設定に使用される。
 */
UCLASS(Abstract, Const, Transient)
class GSCORE_API UGSCPickerTemplate_SettingSource : public UObject
{
public:
	GENERATED_BODY()

public:
	/**
	 * 設定へアクセス可能なオブジェクトを取得する関数の型を指定するためのデリゲート
	 */
	DECLARE_DYNAMIC_DELEGATE_RetVal(UObject*, FStaticContextTemplate);

};


/**
 * 様々な変更可能パラメーターを所有する設定オブジェクトへアクセスするためのデータ
 * 
 * Tips:
 *  この構造体は Developer 設定内で使用するためクラスの指定に FSoftClassPath を使用しています。
 */
USTRUCT(BlueprintType)
struct GSCORE_API FGSCPicker_SettingSource
{
public:
	GENERATED_BODY()

	//
	// この構造体型の初期状態のデータ
	//
	static const FGSCPicker_SettingSource Empty;

public:
	//
	// パラメータにアクセス可能なクラス
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FSoftClassPath SourceClass;

	//
	// SourceClass のオブジェクトからアクセスする関数の名前
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName FunctionName{ NAME_None };

public:
	FGSCPicker_SettingSource() = default;

	FGSCPicker_SettingSource(FSoftClassPath InSourceClass, FName InFunctionName)
		: SourceClass(InSourceClass)
		, FunctionName(InFunctionName)
	{
	}

};


