// Copyright (C) 2023 owoDra

#pragma once

#include "Definition/Format/GSCFormatBase.h"

#include "GSCFormat_Collection.generated.h"

class UGSCData_Setting;
class UGSCFilterBase;


/**
 * 設定項目をまとめるコレクションデータのベースクラス
 */
UCLASS(Abstract, meta = (DisplayName = "FormatBase Collection"))
class GSCORE_API UGSCCollectionFormatBase : public UGSCFormatBase
{
public:
	GENERATED_BODY()

public:
	//
	// コレクション内の SettingData のリスト
	// 
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Collection")
	TArray<TSoftObjectPtr<UGSCData_Setting>> SettingDatas;

public:
	virtual bool IsSelectable() const override { return false; }


public:
	/**
	 * Collection 内の SettingData を取得する
	 * 
	 * Tips:
	 *  Filter を設定することで取得する SettingData をフィルタリングすることができる
	 */
	virtual void GetSettingDataFiltered(TArray<UGSCData_Setting*>& OutDatas, const UGSCFilterBase* Filter = nullptr) const;

};


/**
 * 設定項目をまとめるコレクションデータ
 * 
 * Tips:
 *  このフォーマットデータは主に設定メニューの設定項目ごとのカテゴリーの定義に使用されます。
 *  例えば、ゲームプレイ、グラフィック、オーディオ、コントロール などのカテゴリーごとの設定項目のリスト。
 * 
 * ╔[設定メニュー]
 * ╚╦[ゲームプレイ カテゴリー]
 *  ╠[グラフィック カテゴリー]
 *  ╠[オーディオ 　カテゴリー]
 *  ╚[コントロール カテゴリー]
 */
UCLASS(meta = (DisplayName = "Format Collection Category"))
class GSCORE_API UGSCFormat_CollectionCategory final : public UGSCCollectionFormatBase
{
public:
	GENERATED_BODY()

public:
	virtual bool IsSelectable() const override { return false; }

};


/**
 * 設定項目をまとめるコレクションデータの亜種
 *
 * Tips:
 *  このフォーマットデータは主に設定メニューのカテゴリー内でのセクションを定義するのに使用します。
 *  例えば、ゲームプレイカテゴリー内の アクセシビリティ、ユーザーインターフェイス などのセクションごとの設定項目のリスト。
 *  
 * ╔[設定メニュー]
 * ╚╦═╦[ゲームプレイ カテゴリー]
 *  ║ ╚╦[アクセシビリティ 　　　　セクション]
 *  ║  ╚[ユーザーインターフェイス セクション]
 *  ╠[グラフィック カテゴリー]
 *  ╠[オーディオ 　カテゴリー]
 *  ╚[コントロール カテゴリー]
 */
UCLASS(meta = (DisplayName = "Format Collection Section"))
class GSCORE_API UGSCFormat_CollectionSection final : public UGSCCollectionFormatBase
{
public:
	GENERATED_BODY()

protected:
	//
	// この設定項目に対して更新リクエストが来た際にコレクション内の SettingData に対して更新リクエストを拡散するか
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Collection")
	bool bShouldBroadcastRefreshRequest{ false };

protected:
	virtual void HandleRefreshSettingRequest() override;

};
