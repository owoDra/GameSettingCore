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
	//
	// 設定の名前
	// 
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Info")
	FText Name;

	//
	// 設定の説明
	// 
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Info")
	FText Discription;

protected:
	//
	// この設定に関連する SettingData
	// 
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Relationship")
	TArray<TObjectPtr<UGSCData_Setting>> RelatedSettings;

public:
	/**
	 * 設定の GetterSource を取得する
	 */
	virtual const FGSCPicker_PropertySource& GetGetterSource() const { return FGSCPicker_PropertySource::Empty; }

	/**
	 * 設定の SetterSource を取得する
	 */
	virtual const FGSCPicker_PropertySource& GetSetterSource() const { return FGSCPicker_PropertySource::Empty; }

	/**
	 * 設定の DesiredValueSource を取得する
	 */
	virtual const FGSCPicker_PropertySource& GetDesiredValueSource() const { return FGSCPicker_PropertySource::Empty; }


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


protected:
	/**
	 * RelatedSettings で指定した SettingData に対して状態の更新を行う
	 */
	virtual void TryRefreshRelatedSettings();

	/**
	 * DesiredValueSource が有効か確認し、有効ならばその値に設定する
	 */
	virtual void TrySetPropertyToDesiredValue();

};
