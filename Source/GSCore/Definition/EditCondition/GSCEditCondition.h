// Copyright (C) 2023 owoDra

#pragma once

#include "UObject/Object.h"

#include "GSCEditableState.h"

#include "GSCEditCondition.generated.h"


/**
 * ユーザーが変更可能なゲーム設定の変更可能条件を定義する
 */
UCLASS(Abstract, Blueprintable, DefaultToInstanced, EditInlineNew, Within = "GSCData_Setting")
class GSCORE_API UGSCEditCondition : public UObject
{
public:
	GENERATED_BODY()

public:
	/**
	 * 現在の設定の編集可能状態を確認する
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintPure, Category = "EditCondition", meta = (DisplayName = "GatherEditState"))
	FGSCEditableState K2_GatherEditableState() const;
	virtual FGSCEditableState GatherEditableState() const
	{
		return K2_GatherEditableState();
	}

};
