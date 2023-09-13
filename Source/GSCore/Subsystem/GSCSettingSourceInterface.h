// Copyright (C) 2023 owoDra

#pragma once

#include "UObject/Interface.h"

#include "GSCSettingSourceInterface.generated.h"


/** 
 * GameSettingsCore に設定の値を所有しているオブジェクトだと認識させるためのインターフェイス
 * 
 * Tips:
 *  GSCSubsystem はこのインターフェイスを通して汎用的な様々なアクションを呼び出すことができる
 * 
 * 例:
 *  設定の適用(保存)、設定のリセット、設定の再読み込み など
 */
UINTERFACE(BlueprintType, Blueprintable, meta = (DisplayName = "SettingSourceInterface"))
class GSCORE_API UGSCSettingSourceInterface : public UInterface
{
public:
	GENERATED_BODY()

};

class GSCORE_API IGSCSettingSourceInterface
{
public:
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "GameSettings")
	bool RequestApplySettings();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "GameSettings")
	bool RequestResetSettings();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "GameSettings")
	bool RequestReloadSettings();

};
