// Copyright (C) 2023 owoDra

#pragma once

#include "Definition/Format/GSCFormatBase.h"

#include "Definition/Picker/GSCPicker_PropertySource.h"

#include "GSCPropertyFormatBase.generated.h"


/**
 * 設定定義データのプロパティ関係の定義内容を決めるためのベースクラス
 */
UCLASS(Abstract)
class GSCORE_API UGSCPropertyFormatBase : public UGSCFormatBase
{
public:
	GENERATED_BODY()
public:

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) override;
#endif // WITH_EDITOR

public:
	//
	// 他の SettingData などによって SettingData の更新リクエストがあったときに UI に対して設定値の更新を知らせるデリゲート
	//
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGSCRefreshSettingRequestDelegate, bool, bCalledByOther);
	UPROPERTY(BlueprintAssignable, Transient)
	FGSCRefreshSettingRequestDelegate OnRequestRefreshSetting;

protected:
	//
	// 設定の説明
	// 
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Info")
	FText Description;

protected:
	//
	// この設定に関連する SettingData
	// 
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Relationship")
	TArray<TSoftObjectPtr<UGSCData_Setting>> RelatedSettings;

public:
	virtual bool IsSelectable() const override { return true; }

	virtual FString ToString() const override { return Name.ToString() + Description.ToString(); }

	/**
	 * 説明を取得する
	 */
	virtual const FText& GetDescription() const { return Description; }

	/**
	 * 設定の GetterSource を取得する
	 */
	virtual const FGSCPicker_PropertySource& GetGetterSource() const { return FGSCPicker_PropertySource::Empty; }

	/**
	 * 設定の SetterSource を取得する
	 */
	virtual const FGSCPicker_PropertySource& GetSetterSource() const { return FGSCPicker_PropertySource::Empty; }

	/**
	 * 設定の DefaultSource を取得する
	 */
	virtual const FGSCPicker_PropertySource& GetDefaultSource() const { return FGSCPicker_PropertySource::Empty; }


protected:
	virtual void HandleRefreshSettingRequest() override;

	/**
	 * この Format によって定義されたプロパティ設定項目の値が変更されたときに実行する
	 */
	virtual void HandleSettingValueChanged();


protected:
	/**
	 * この Format によって定義されたプロパティ設定項目の値を変更する
	 * 
	 * 注意:
	 *  この関数では値を FString 型として設定するため派生クラスでそれぞれの型に対応した
	 *  ラッパーを作成することを推奨します。
	 */
	virtual bool SetPropertyValueInternal(const FGSCPicker_PropertySource& SetterSource, FString NewValue);

	/**
	 * この Format によって定義されたプロパティ設定項目の値を取得する
	 * 
	 * 注意:
	 *  この関数では値を FString 型として取得するため派生クラスでそれぞれの型に対応した
	 *  ラッパーを作成することを推奨します。
	 */
	virtual bool GetPropertyValueInternal(const FGSCPicker_PropertySource& GetterSource, FString& OutValue);

	/**
	 * この Format によって定義されたプロパティ設定項目のデフォルト値を取得する
	 *
	 * 注意:
	 *  この関数では値を FString 型として取得するため派生クラスでそれぞれの型に対応した
	 *  ラッパーを作成することを推奨します。
	 */
	virtual bool GetPropertyDefaultInternal(const FGSCPicker_PropertySource& DefaultSource, FString& OutValue);

public:
	/**
	 * この Format によって定義されたプロパティ設定項目の値をデフォルト値に戻す
	 *
	 * 注意:
	 *  内部的に FString 型 を通して設定を変更するため FString への変換が不可能な型の場合は使用できません。
	 */
	virtual bool SetPropertyToDefault();


protected:
	/**
	 * RelatedSettings で指定した SettingData に対して状態の更新を行う
	 */
	virtual void TryRefreshRelatedSettings();

};
