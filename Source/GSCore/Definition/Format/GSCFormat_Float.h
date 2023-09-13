// Copyright (C) 2023 owoDra

#pragma once

#include "GSCFormatBase.h"

#include "GSCFormat_Float.generated.h"


/**
 * エディターで関数の選択を可能にする際に関数を絞り込むための型定義のみ行ったダミーのデリゲートのラッパー
 *
 * Tips:
 *  ここで定義されたデリゲートは主に UGSCFormat_Float 内でのデータソースの設定に使用される。
 */
UCLASS(Abstract, Const, Transient)
class UGSCPickerTemplate_FormatFloat : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Getter 関数の型を指定するためのデリゲート
	 */
	DECLARE_DYNAMIC_DELEGATE_RetVal(float, FGetterTemplate);

	/**
	 * Setter 関数の型を指定するためのデリゲート
	 */
	DECLARE_DYNAMIC_DELEGATE_OneParam(FSetterTemplate, float, Param);

};


/**
 * float(double) 型の設定項目に対する必要情報定義用のフォーマットクラス
 */
UCLASS()
class UGSCFormat_Float : public UGSCFormatBase
{
public:
	GENERATED_BODY()

protected:
	//
	// 設定の値の Getter 関数名
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DataSource", meta = (
		DisplayAfter = "StaticContextSource",
		GetterFuncTemplate = "/Script/GSCore.GSCPickerTemplate_FormatFloat::GetterTemplate__DelegateSignature"))
	FGSCPicker_PropertySource GetterSource{ FGSCPicker_PropertySource::Empty };

	//
	// 設定の値の Setter 関数名
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DataSource", meta = (
		DisplayAfter = "StaticContextSource",
		SetterFuncTemplate = "/Script/GSCore.GSCPickerTemplate_FormatFloat::SetterTemplate__DelegateSignature"))
	FGSCPicker_PropertySource SetterSource{ FGSCPicker_PropertySource::Empty };

protected:
	//
	// 設定値の編集可能な最大値
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI Parameter")
	float MaxValue{ 1.0f };

	//
	// 設定値の編集可能な最小値
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI Parameter")
	float MinValue{ 0.0f };

	//
	// 設定値の編集時のスライダーのステップ
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI Parameter")
	float Step{ 0.1f };

	//
	// 設定値の表示可能な小数点以下の桁数
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI Parameter")
	int32 FractionDigits { 2 };

public:
	virtual FGSCPicker_PropertySource GetGetterSource() const override;
	virtual FGSCPicker_PropertySource GetSetterSource() const override;

};
