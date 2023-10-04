// Copyright (C) 2023 owoDra

#pragma once

#include "UObject/Object.h"

#include "GSCFormatBase.generated.h"


/**
 * 設定定義データの定義内容を決めるためのベースクラス
 */
UCLASS(Abstract, DefaultToInstanced, EditInlineNew, NotBlueprintable, Within = "GSCData_Setting")
class GSCORE_API UGSCFormatBase : public UObject
{
public:
	GENERATED_BODY()

	friend class UGSCData_Setting;

protected:
	//
	// 設定項目の名前
	// 
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Info")
	FText Name;

public:
	/**
	 * 名前を取得する
	 */
	virtual const FText& GetName() const { return Name; }

	/**
	 * 設定項目に関する文字列を FString 型で返す
	 * 
	 * Tips:
	 *  この文字列は一般的には設定項目の名前であったり、名前と説明を合わせたものである。
	 *  この文字列は設定項目リストなど複数設定が項目がある際に特定の設定項目を検索するときに便利です。
	 */
	virtual FString ToString() const { return GetName().ToString(); }

	/**
	 * この Format によって定義された設定項目が設定メニューで選択可能な項目かどうかを返す
	 */
	virtual bool IsSelectable() const { return false; }

protected:
	/**
	 * この Format を所有する SettingData に対して更新リクエストが行われたときに実行する
	 */
	virtual void HandleRefreshSettingRequest() {}

};
