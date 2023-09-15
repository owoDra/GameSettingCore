// Copyright (C) 2023 owoDra

#pragma once

#include "Engine/DataAsset.h"

#include "GameplayTagContainer.h"

#include "GSCData_ListVisual.generated.h"

class UUserWidget;
class UGSCFormatBase;
class UGSCData_SettingBase;


/**
 * GSCListView に表示可能なウィジェットクラスを選択するためのデータ
 */
USTRUCT(BlueprintType)
struct FGSCPicker_ListEntry
{
public:
	GENERATED_BODY()
public:
	FGSCPicker_ListEntry() {}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (MustImplement = "GSCListEntryWidgetInterface"))
	TSubclassOf<UUserWidget> WidgetClass{ nullptr };

};


/**
 * GSCListView で使用する設定の種類ごとに使用するウィジェットを決めるデータアセット
 */
UCLASS(BlueprintType, Const)
class GSCORE_API UGSCData_ListVisual : public UDataAsset
{
public:
	GENERATED_BODY()

protected:
	//
	// リスト内のセクションごとのヘッダーに使用するウィジェットクラス
	// 
	// Tips:
	//  デフォルトでは GSCListView に表示させようとした SettingCollection 内のサブ SettingCollection の
	//  名前がヘッダーとして使用される。
	//  このヘッダー名決定のルールは GSCListView から派生クラスを作成することで変更可能。
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "ListView")
	FGSCPicker_ListEntry EntryWidgetForHeader;

	//
	// 設定定義データのフォーマットごとに使用するウィジェットクラスのマッピングリスト
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "ListView")
	TMap<TSubclassOf<UGSCFormatBase>, FGSCPicker_ListEntry> EntryWidgetsForFormat;

	//
	// 設定定義データの SettingTag ごとに使用するウィジェットクラスのマッピングリスト
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "ListView")
	TMap<FGameplayTag, FGSCPicker_ListEntry> EntryWidgetsForTag;

public:
	/**
	 * 指定した Data に対応する EntryWidget のクラスを返す
	 * 有効なクラスが見つからなかった場合は nullptr を返す
	 */
	TSubclassOf<UUserWidget> GetEntryWidgetForData(const UGSCData_SettingBase* InData);

};
