// Copyright (C) 2023 owoDra

#pragma once

#include "Definition/Format/GSCPropertyFormatBase.h"

#include "GSCFormat_Switch.generated.h"


/**
 * エディターで関数の選択を可能にする際に関数を絞り込むための型定義のみ行ったダミーのデリゲートのラッパー
 */
UCLASS(Abstract, Const, Transient)
class UGSCPickerTemplate_FormatSwitch : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * bool 型 Getter 関数を指定するためのデリゲート
	 */
	DECLARE_DYNAMIC_DELEGATE_RetVal(bool, FBoolGetterTemplate);

	/**
	 * bool 型 Setter 関数を指定するためのデリゲート
	 */
	DECLARE_DYNAMIC_DELEGATE_OneParam(FBoolSetterTemplate, bool, Param);

};


/**
 * スイッチタイプの設定項目に対する必要情報定義用のフォーマットのベースクラス
 */
UCLASS(Abstract, meta = (DisplayName = "FormatBase Switch"))
class GSCORE_API UGSCSwitchPropertyFormatBase : public UGSCPropertyFormatBase
{
public:
	GENERATED_BODY()
public:
	UGSCSwitchPropertyFormatBase();

protected:
	//
	// 値が false の時の表示テキスト
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI Parameter")
	FText TrueText;

	//
	// 値が true の時の表示テキスト
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI Parameter")
	FText FalseText;


public:
	/**
	 * 値が true の時の表示テキストを返す
	 */
	virtual FText GetTrueText() const { return TrueText; }

	/**
	 * 値が false の時の表示テキストを返す
	 */
	virtual FText GetFalseText() const { return FalseText; }


public:
	/**
	 * この Format によって定義されたプロパティ設定項目の値を変更する
	 */
	virtual bool SetPropertyValue(bool NewValue);

	/**
	 * この Format によって定義されたプロパティ設定項目の値を取得する
	 */
	virtual bool GetPropertyValue(bool& OutValue);

	/**
	 * この Format によって定義されたプロパティ設定項目のデフォルト値を取得する
	 */
	virtual bool GetPropertyDefault(bool& OutValue);

};


/**
 * bool 型のスイッチタイプの設定項目に対する必要情報定義用のフォーマットクラス
 */
UCLASS(meta = (DisplayName = "Format Switch Boolean"))
class GSCORE_API UGSCFormat_SwitchBool : public UGSCSwitchPropertyFormatBase
{
public:
	GENERATED_BODY()

protected:
	//
	// 設定の値の Getter 関数名
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DataSource", meta = (
		GetterFuncTemplate = "/Script/GSCore.GSCPickerTemplate_FormatSwitch::GetterTemplate__DelegateSignature"))
	FGSCPicker_PropertySource GetterSource{ FGSCPicker_PropertySource::Empty };

	//
	// 設定の値の Setter 関数名
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DataSource", meta = (
		SetterFuncTemplate = "/Script/GSCore.GSCPickerTemplate_FormatSwitch::SetterTemplate__DelegateSignature"))
	FGSCPicker_PropertySource SetterSource{ FGSCPicker_PropertySource::Empty };

	//
	// 設定のデフォルト値の Getter 関数名
	// 
	// Tips:
	//  基本的には GetterSource と同じもの
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DataSource", meta = (
		GetterFuncTemplate = "/Script/GSCore.GSCPickerTemplate_FormatSwitch::GetterTemplate__DelegateSignature"))
	FGSCPicker_PropertySource DefaultSource{ FGSCPicker_PropertySource::Empty };

public:
	virtual const FGSCPicker_PropertySource& GetGetterSource() const override;
	virtual const FGSCPicker_PropertySource& GetSetterSource() const override;
	virtual const FGSCPicker_PropertySource& GetDefaultSource() const override;

};
