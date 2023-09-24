// Copyright (C) 2023 owoDra

#pragma once

#include "Definition/Format/GSCPropertyFormatBase.h"

#include "GSCFormat_Scalar.generated.h"


/**
 * エディターで関数の選択を可能にする際に関数を絞り込むための型定義のみ行ったダミーのデリゲートのラッパー
 */
UCLASS(Abstract, Const, Transient)
class UGSCPickerTemplate_FormatScalar : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * float 型 Getter 関数を指定するためのデリゲート
	 */
	DECLARE_DYNAMIC_DELEGATE_RetVal(float, FFloatGetterTemplate);

	/**
	 * float 型 関数を指定するためのデリゲート
	 */
	DECLARE_DYNAMIC_DELEGATE_OneParam(FFloatSetterTemplate, float, Param);

	/**
	 * int 型 Getter 関数を指定するためのデリゲート
	 */
	DECLARE_DYNAMIC_DELEGATE_RetVal(int32, FIntGetterTemplate);

	/**
	 * int 型 関数を指定するためのデリゲート
	 */
	DECLARE_DYNAMIC_DELEGATE_OneParam(FIntSetterTemplate, int32, Param);
};


/**
 * 数値タイプの設定項目に対する必要情報定義用のフォーマットのベースクラス
 */
UCLASS(Abstract, meta = (DisplayName = "FormatBase Scalar"))
class GSCORE_API UGSCScalarPropertyFormatBase : public UGSCPropertyFormatBase
{
public:
	GENERATED_BODY()

public:
	/**
	 * 設定値の範囲を返す
	 */
	virtual void GetValueRange(float& OutMax, float& OutMin) const { OutMax = 1.0f; OutMin = 0.0f; }

	/**
	 * 設定値の編集時のスライダーのステップを返す
	 */
	virtual float GetStep() const { return 0.0f; }

	/**
	 * 設定値の表示可能な小数点以下の桁数を返す
	 */
	virtual int32 GetFractionDigits() const { return 0; }

public:
	/**
	 * この Format によって定義されたプロパティ設定項目の値を変更する
	 */
	virtual bool SetPropertyValue(float NewValue);

	/**
	 * この Format によって定義されたプロパティ設定項目の値を取得する
	 */
	virtual bool GetPropertyValue(float& OutValue);

	/**
	 * この Format によって定義されたプロパティ設定項目のデフォルト値を取得する
	 */
	virtual bool GetPropertyDefault(float& OutValue);

};


/**
 * float 型の数値タイプの設定項目に対する必要情報定義用のフォーマットクラス
 */
UCLASS(meta = (DisplayName = "Format Scalar Float"))
class GSCORE_API UGSCFormat_ScalarFloat : public UGSCScalarPropertyFormatBase
{
public:
	GENERATED_BODY()

protected:
	//
	// 設定の値の Getter 関数名
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DataSource", meta = (
		GetterFuncTemplate = "/Script/GSCore.GSCPickerTemplate_FormatScalar::FloatGetterTemplate__DelegateSignature"))
	FGSCPicker_PropertySource GetterSource{ FGSCPicker_PropertySource::Empty };

	//
	// 設定の値の Setter 関数名
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DataSource", meta = (
		SetterFuncTemplate = "/Script/GSCore.GSCPickerTemplate_FormatScalar::FloatSetterTemplate__DelegateSignature"))
	FGSCPicker_PropertySource SetterSource{ FGSCPicker_PropertySource::Empty };

	//
	// 設定のデフォルト値の Getter 関数名
	// 
	// Tips:
	//  基本的には GetterSource と同じもの
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DataSource", meta = (
		GetterFuncTemplate = "/Script/GSCore.GSCPickerTemplate_FormatScalar::FloatGetterTemplate__DelegateSignature"))
	FGSCPicker_PropertySource DefaultSource{ FGSCPicker_PropertySource::Empty };

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
	virtual const FGSCPicker_PropertySource& GetGetterSource() const override;
	virtual const FGSCPicker_PropertySource& GetSetterSource() const override;
	virtual const FGSCPicker_PropertySource& GetDefaultSource() const override;

	virtual void GetValueRange(float& OutMax, float& OutMin) const override;
	virtual float GetStep() const override;
	virtual int32 GetFractionDigits() const override;
};


/**
 * int 型の数値タイプの設定項目に対する必要情報定義用のフォーマットクラス
 */
UCLASS(meta = (DisplayName = "Format Scalar Integer"))
class GSCORE_API UGSCFormat_ScalarInt : public UGSCScalarPropertyFormatBase
{
public:
	GENERATED_BODY()

protected:
	//
	// 設定の値の Getter 関数名
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DataSource", meta = (
		GetterFuncTemplate = "/Script/GSCore.GSCPickerTemplate_FormatScalar::IntGetterTemplate__DelegateSignature"))
	FGSCPicker_PropertySource GetterSource{ FGSCPicker_PropertySource::Empty };

	//
	// 設定の値の Setter 関数名
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DataSource", meta = (
		SetterFuncTemplate = "/Script/GSCore.GSCPickerTemplate_FormatScalar::IntSetterTemplate__DelegateSignature"))
	FGSCPicker_PropertySource SetterSource{ FGSCPicker_PropertySource::Empty };

	//
	// 設定のデフォルト値の Getter 関数名
	// 
	// Tips:
	//  基本的には GetterSource と同じもの
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DataSource", meta = (
		GetterFuncTemplate = "/Script/GSCore.GSCPickerTemplate_FormatScalar::IntGetterTemplate__DelegateSignature"))
	FGSCPicker_PropertySource DefaultSource{ FGSCPicker_PropertySource::Empty };

protected:
	//
	// 設定値の編集可能な最大値
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI Parameter")
	int32 MaxValue{ 100 };

	//
	// 設定値の編集可能な最小値
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI Parameter")
	int32 MinValue{ 0 };

	//
	// 設定値の編集時のスライダーのステップ
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI Parameter")
	int32 Step{ 1 };

public:
	virtual const FGSCPicker_PropertySource& GetGetterSource() const override;
	virtual const FGSCPicker_PropertySource& GetSetterSource() const override;
	virtual const FGSCPicker_PropertySource& GetDefaultSource() const override;

	virtual void GetValueRange(float& OutMax, float& OutMin) const override;
	virtual float GetStep() const override;
	virtual int32 GetFractionDigits() const override;

};
