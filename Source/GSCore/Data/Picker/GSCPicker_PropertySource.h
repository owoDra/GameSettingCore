// Copyright (C) 2023 owoDra

#pragma once

#include "UObject/Object.h"

#include "GSCPicker_PropertySource.generated.h"


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

};


/**
 * 設定項目の定義時に使用する設定のプロパティへアクセスするためのデータソース
 * 
 * Tips:
 *  この構造体の変数定義では UPROPERTY マクロの Meta指定子 を用いて候補で表示される
 *  Getter/Setter 関数の種類を決めることができます。使用できる Meta指定子 は以下の通りです。
 *  
 *  - GetterFuncTemplate
 *  - SetterFuncTemplate
 *  
 *  引数に DYNAMIC_DELEGATE への参照パスを入力することで表示する関数を指定できる。
 * 
 * 例:
 *  UPROPERTY(meta = (GetterFuncTemplate = "/Script/ModuleName.ClassName::DelegateName__DelegateSignature"))
 */
USTRUCT(BlueprintType)
struct GSCORE_API FGSCPicker_PropertySource
{
public:
	GENERATED_BODY()

	//
	// この構造体型の初期状態のデータ
	//
	static const FGSCPicker_PropertySource Empty;

public:
	//
	// 関数にアクセス可能なデータソースの名前
	// 
	// Tips:
	//  このデータソース名は GameSettingsCore のデベロッパー設定で登録したものになります。
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SettingSourceName{ NAME_None };

	//
	// プロパティにアクセスする関数の名前
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName FunctionName{ NAME_None };

public:
	FGSCPicker_PropertySource() = default;

	FGSCPicker_PropertySource(FName InSettingSourceName, FName InFunctionName)
		: SettingSourceName(InSettingSourceName)
		, FunctionName(InFunctionName)
	{
	}

public:
	/**
	 * 現在のデータが有効かどうかを返す
	 */
	bool IsValid() const;

};


