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

public:
	/**
	 * この Format を所有する SettingData で更新リクエストがあった際に呼び出される
	 */
	virtual void HandleRefreshSettingRequest() {}

};
