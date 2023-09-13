// Copyright (C) 2023 owoDra

#pragma once

#include "Subsystems/EngineSubsystem.h"

#include "Definition/Picker/GSCPicker_SettingSource.h"

#include "GSCSubsystem.generated.h"

class UObject;
class UGSCData_SettingDefinition;


/**
 * デベロッパー設定で登録されたデータソースとなるオブジェクトに対して
 * 定義された設定の値をまとめて管理するためのサブシステム
 */
UCLASS(BlueprintType, meta = (ScriptName = "GameSettingsCoreSubsystem", DisplayName = "GSC"))
class GSCORE_API UGSCSubsystem : public UEngineSubsystem
{
public:
	GENERATED_BODY()

	using FSettingSourceStaticContext = UGSCPickerTemplate_SettingSource::FStaticContextTemplate;

protected:
	//
	// キャッシュ済みのデータソースオブジェクトへアクセスるためのデリゲートリスト
	// 
	UPROPERTY(Transient)
	TMap<FName, UGSCPickerTemplate_SettingSource::FStaticContextTemplate> CachedSettingSources;

public:
	/**
	 * デベロッパー設定で登録されたデータソースオブジェクトを取得する
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameSettings")
	UObject* LoadOrGetSettingSourceObject(FName SettingSourceName);


public:
	/**
	 * 登録されているすべての SettingSource に対して 設定適用リクエストを行う
	 */
	UFUNCTION(BlueprintCallable, Category = "GameSettings|Requests")
	void RequestApplySettingsForAll();

	/**
	 * 特定の SettingSource に対して 設定適用リクエストを行う
	 */
	UFUNCTION(BlueprintCallable, Category = "GameSettings|Requests")
	bool RequestApplySettings(FName SettingSourceName);

public:
	/**
	 * 登録されているすべての SettingSource に対して 設定リセットリクエストを行う
	 */
	UFUNCTION(BlueprintCallable, Category = "GameSettings|Requests")
	void RequestResetSettingsForAll();

	/**
	 * 特定の SettingSource に対して 設定リセットリクエストを行う
	 */
	UFUNCTION(BlueprintCallable, Category = "GameSettings|Requests")
	bool RequestResetSettings(FName SettingSourceName);

public:
	/**
	 * 登録されているすべての SettingSource に対して 設定再読み込みリクエストを行う
	 */
	UFUNCTION(BlueprintCallable, Category = "GameSettings|Requests")
	void RequestReloadSettingsForAll();

	/**
	 * 特定の SettingSource に対して 設定再読み込みリクエストを行う
	 */
	UFUNCTION(BlueprintCallable, Category = "GameSettings|Requests")
	bool RequestReloadSettings(FName SettingSourceName);


public:
	/**
	 * 指定した設定定義に応じた設定の値を FString 型で返す
	 */
	UFUNCTION(BlueprintCallable, Category = "GameSettings|Properties")
	bool GetSettingValueAsString(const UGSCData_SettingDefinition* Definition, FString& OutValue);

	/**
	 * 指定した設定定義に応じた設定の値を FString 型で設定する
	 */
	UFUNCTION(BlueprintCallable, Category = "GameSettings|Properties")
	bool SetSettingValueFromString(const UGSCData_SettingDefinition* Definition, FString NewValue);

};
