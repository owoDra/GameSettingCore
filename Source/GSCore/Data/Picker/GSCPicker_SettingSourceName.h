// Copyright (C) 2023 owoDra

#pragma once

#include "UObject/Object.h"

#include "GSCPicker_SettingSourceName.generated.h"


/**
 * 設定項目の定義時に使用する設定のプロパティへアクセスするための SettingSource の名前データ
 */
USTRUCT(BlueprintType)
struct GSCORE_API FGSCPicker_SettingSourceName
{
public:
	GENERATED_BODY()

	//
	// この構造体型の初期状態のデータ
	//
	static const FGSCPicker_SettingSourceName Empty;

public:
	//
	// 関数にアクセス可能なデータソースの名前
	// 
	// Tips:
	//  このデータソース名は GameSettingsCore のデベロッパー設定で登録したものになります。
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ShowOnlyInnerProperties))
	FName Name{ NAME_None };

public:
	FGSCPicker_SettingSourceName() = default;

	FGSCPicker_SettingSourceName(FName InSettingSourceName)
		: Name(InSettingSourceName)
	{
	}

public:
	/**
	 * 現在のデータが有効かどうかを返す
	 */
	bool IsValid() const;

};


