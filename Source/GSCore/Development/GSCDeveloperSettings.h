// Copyright (C) 2023 owoDra

#pragma once

#include "Engine/DeveloperSettings.h"

#include "Definition/Picker/GSCPicker_SettingSource.h"

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
	// ユーザーがー変更可能なゲーム設定の値自体を格納しているオブジェクトへアクセスするためのデータソース登録
	// 
	// 例:
	//  GameUserSettings, GameSave など
	// 
	// Key	 : データソースの識別名(任意の文字列)
	// Value : オブジェクトのクラスとアクセスに用いる static 関数の名前
	// 
	// 注意:
	//  候補表示は SourceClass で指定したクラス内で定義された static 関数のみを表示します。
	// 
	// 表示される例:
	//  GameUserSettings の場合クラス内で static 関数 GetGameUserSettings() が定義されているので候補に表示されます。
	// 
	// 表示されない例:
	//  GameInstance の場合はクラス内に static 関数 GetGameInstance() が定義されていないので候補に表示されません。
	//
	UPROPERTY(Config, EditAnywhere, Category = "DataSources", meta = (ForceInlineRow, ConfigRestartRequired = true))
	TMap<FName, FGSCPicker_SettingSource> SettingSources;

};
