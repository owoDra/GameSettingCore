// Copyright (C) 2023 owoDra

#pragma once

#include "Subsystems/EngineSubsystem.h"

#include "Definition/Picker/GSCPicker_SettingSource.h"
#include "Definition/Picker/GSCPicker_PropertySource.h"
#include "Definition/Picker/GSCPicker_SettingSourceName.h"

#include "GSCSubsystem.generated.h"

class UObject;
class UGSCData_Setting;
class UGSCPropertyFormatBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGSCSettingSourceDirtyChangeDelegate, FName, SettingSourceName, bool, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGSCSettingSourceActionDelegate, FName, SettingSourceName);


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

	//
	// 編集未適用状態の SettingSource の名前リスト
	// 
	UPROPERTY(Transient)
	TSet<FName> DirtySettingSources;

public:
	//
	// SettingSource の編集未適応状態が変更されたことを知らせるデリゲート
	//
	UPROPERTY(BlueprintAssignable, Transient)
	FGSCSettingSourceDirtyChangeDelegate OnSettingSourceDirtyChanged;

	//
	// SettingSource を適用させたことを知らせるデリゲート
	//
	UPROPERTY(BlueprintAssignable, Transient)
	FGSCSettingSourceActionDelegate OnSettingSourceApplied;

	//
	// SettingSource の復元させたことを知らせるデリゲート
	//
	UPROPERTY(BlueprintAssignable, Transient)
	FGSCSettingSourceActionDelegate OnSettingSourceReloaded;

	//
	// SettingSource のリセットさせたことを知らせるデリゲート
	//
	UPROPERTY(BlueprintAssignable, Transient)
	FGSCSettingSourceActionDelegate OnSettingSourceReset;

public:
	/**
	 * このサブシステムを取得する
	 */
	static UGSCSubsystem* Get();

	/**
	 * デベロッパー設定で登録された SettingSource オブジェクトを取得する
	 */
	UFUNCTION(BlueprintCallable, Category = "Utilities")
	UObject* LoadOrGetSettingSourceObject(FName SettingSourceName);

	/**
	 * SettingSourceName が登録された SettingSourceName に含まれているかを返す
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Utilities")
	bool IsValidSettingSourceName(FName SettingSourceName) const;


public:
	/**
	 * 指定した SettingSource が編集未適用状態かを返す
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameSettings")
	bool IsDirty(FName SettingSourceName) const;

	/**
	 * 指定した複数の SettingSource の中に編集未適用状態のものがあるかを返す
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameSettings")
	bool ContainDirty(const TArray<FName>& SettingSourceName) const;

	bool ContainDirty(const TArray<FGSCPicker_SettingSourceName>& SettingSourceName) const;

protected:
	/**
	 * 指定した SettingSource に編集未適用フラグを付ける
	 */
	void MarkDirty(FName SettingSourceName);

	/**
	 * 指定した SettingSource の編集未適用フラグを消す
	 */
	void ClearDirty(FName SettingSourceName);


public:
	/**
	 * 特定の SettingSource に対して 設定適用リクエストを行う
	 */
	UFUNCTION(BlueprintCallable, Category = "GameSettings|Requests")
	bool RequestApplySetting(FName SettingSourceName);

	/**
	 * 複数の SettingSource に対して 設定適用リクエストを行う
	 */
	UFUNCTION(BlueprintCallable, Category = "GameSettings|Requests")
	bool RequestApplySettings(const TArray<FName>& SettingSourceNames);

	bool RequestApplySettings(const TArray<FGSCPicker_SettingSourceName>& SettingSourceNames);

	/**
	 * 特定の SettingSource に対して 設定リセットリクエストを行う
	 */
	UFUNCTION(BlueprintCallable, Category = "GameSettings|Requests")
	bool RequestResetSetting(FName SettingSourceName);

	/**
	 * 複数の SettingSource に対して 設定リセットリクエストを行う
	 */
	UFUNCTION(BlueprintCallable, Category = "GameSettings|Requests")
	bool RequestResetSettings(const TArray<FName>& SettingSourceNames);

	bool RequestResetSettings(const TArray<FGSCPicker_SettingSourceName>& SettingSourceNames);

	/**
	 * 特定の SettingSource に対して 設定再読み込みリクエストを行う
	 */
	UFUNCTION(BlueprintCallable, Category = "GameSettings|Requests")
	bool RequestReloadSetting(FName SettingSourceName);

	/**
	 * 複数の SettingSource に対して 設定再読み込みリクエストを行う
	 */
	UFUNCTION(BlueprintCallable, Category = "GameSettings|Requests")
	bool RequestReloadSettings(const TArray<FName>& SettingSourceNames);

	bool RequestReloadSettings(const TArray<FGSCPicker_SettingSourceName>& SettingSourceNames);


public:
	/**
	 * 指定した設定定義に応じた設定の値を FString 型で返す
	 */
	bool GetSettingValueAsString(UGSCPropertyFormatBase* PropertyFormat, FString& OutValue);

	bool GetSettingValueAsString(const UObject* Context, const FGSCPicker_PropertySource& GetterSource, FString& OutValue);

	/**
	 * 指定した設定定義に応じた設定の値を FString 型で設定する
	 */
	bool SetSettingValueFromString(UGSCPropertyFormatBase* PropertyFormat, FString NewValue);

	/**
	 * 指定した設定定義に応じた設定のデフォルト値を FString 型で返す
	 */
	bool GetSettingDefaultAsString(UGSCPropertyFormatBase* PropertyFormat, FString& OutValue);

	bool GetSettingDefaultAsString(const UObject* Context, const FGSCPicker_PropertySource& DefaultSource, FString& OutValue);

	/**
	 * 指定した設定定義に応じた設定の値を デフォルト値に戻す
	 * 
	 * 注意:
	 *  内部的に FString 型 を通して設定を変更するため FString への変換が不可能な型の場合は使用できません。
	 */
	bool SetSettingValueToDefault(UGSCPropertyFormatBase* PropertyFormat);

};
