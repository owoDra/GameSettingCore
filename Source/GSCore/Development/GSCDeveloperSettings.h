// Copyright (C) 2023 owoDra

#pragma once

#include "Engine/DeveloperSettings.h"

#include "GSCDeveloperSettings.generated.h"


/**
 * ユーザーが変更可能なゲーム設定項目の定義の設定
 */
UCLASS(config = Game, defaultconfig, meta = (DisplayName = "Setting Definition"))
class GSCORE_API UGSCDeveloperSettings : public UDeveloperSettings
{
public:
	GENERATED_BODY()
public:
	UGSCDeveloperSettings();

public:
	//
	// ゲーム設定のカテゴリーごとの設定項目の定義データ
	//
	UPROPERTY(Config, EditAnywhere, Category = "GameSettings", meta = (AllowedClasses = "/Script/GSCore.GSCData_SettingCollection"))
	TArray<FSoftObjectPath> Collections;

};
