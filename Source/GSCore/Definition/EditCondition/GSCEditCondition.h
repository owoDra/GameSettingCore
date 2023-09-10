// Copyright (C) 2023 owoDra

#pragma once

#include "UObject/Object.h"

#include "GSCEditCondition.generated.h"


/**
 * ユーザーが変更可能なゲーム設定の編集可能状態
 */
UENUM(BlueprintType)
enum class EGSCSettingEditState : uint8
{
	Editable,	// 表示され編集可能

	NotEditable,// 表示されているが編集不可

	Collapsed	// 表示されず編集も不可
};


/**
 * ユーザーが変更可能なゲーム設定の変更可能条件を定義する
 */
UCLASS(Abstract, Blueprintable, DefaultToInstanced, EditInlineNew)
class GSCORE_API UGSCEditCondition : public UObject
{
public:
	GENERATED_BODY()

public:
	/**
	 * 現在の設定の編集可能状態を確認する
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintPure, Category = "EditCondition", meta = (DisplayName = "GatherEditState"))
	EGSCSettingEditState K2_GatherEditState() const;
	virtual EGSCSettingEditState GatherEditState() const
	{
		return K2_GatherEditState();
	}

};
