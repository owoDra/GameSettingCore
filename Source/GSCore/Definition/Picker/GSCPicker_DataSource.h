// Copyright (C) 2023 owoDra

#pragma once

#include "UObject/Object.h"

#include "GSCPicker_DataSource.generated.h"


/**
 * 設定項目の定義時に使用する設定のパラメータソースへアクセスするためのデータ
 * 
 * Tips:
 *  この構造体の変数定義では UPROPERTY マクロの Meta指定子 を用いて候補で表示される
 *  Getter/Setter 関数の種類を決めることができます。使用できる Meta指定子 は以下の通りです。
 *  
 *  - ContextFuncTemplate (この指定しを使用する場合、表示されるのは static 関数のみになる)
 *  - GetterFuncTemplate
 *  - SetterFuncTemplate
 *  
 *  引数に DYNAMIC_DELEGATE への参照パスを入力することで表示する関数を指定できる。
 * 
 * 例:
 *  UPROPERTY(meta = (GetterFuncTemplate = "/Script/ModuleName.ClassName::DelegateName__DelegateSignature"))
 */
USTRUCT(BlueprintType)
struct GSCORE_API FGSCPicker_DataSource
{
public:
	GENERATED_BODY()

	//
	// この構造体型の初期状態のデータ
	//
	static const FGSCPicker_DataSource Empty;

public:
	//
	// パラメータにアクセス可能なクラス
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UClass> SourceClass{ nullptr };

	//
	// SourceClass のオブジェクトからアクセスする関数の名前
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName FunctionName{ NAME_None };

public:
	FGSCPicker_DataSource() = default;

	FGSCPicker_DataSource(UClass* InSourceClass, FName InFunctionName)
		: SourceClass(InSourceClass)
		, FunctionName(InFunctionName)
	{
	}

public:
	/**
	 * 現在のデータが有効かどうかを返す
	 */
	bool IsValid() const { return (SourceClass && FunctionName.IsValid() && !FunctionName.IsNone()); }

};


