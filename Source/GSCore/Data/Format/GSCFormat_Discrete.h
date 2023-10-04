// Copyright (C) 2023 owoDra

#pragma once

#include "Data/Format/GSCPropertyFormatBase.h"

#include "GSCFormat_Discrete.generated.h"


/**
 * エディターで関数の選択を可能にする際に関数を絞り込むための型定義のみ行ったダミーのデリゲートのラッパー
 */
UCLASS(Abstract, Const, Transient)
class UGSCPickerTemplate_FormatDiscrete : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * int 型 Getter 関数を指定するためのデリゲート
	 */
	DECLARE_DYNAMIC_DELEGATE_RetVal(int32, FIntGetterTemplate);

	/**
	 * int 型 Setter 関数を指定するためのデリゲート
	 */
	DECLARE_DYNAMIC_DELEGATE_OneParam(FIntSetterTemplate, int32, Param);

	/**
	 * int 型の選択項目の Getter 関数を指定するためのデリゲート
	 */
	DECLARE_DYNAMIC_DELEGATE_OneParam(FIntDynamicOptionsTemplate, TArray<int32>&, OutParam);

	/**
	 * float 型 Getter 関数を指定するためのデリゲート
	 */
	DECLARE_DYNAMIC_DELEGATE_RetVal(float, FFloatGetterTemplate);

	/**
	 * float 型 Setter 関数を指定するためのデリゲート
	 */
	DECLARE_DYNAMIC_DELEGATE_OneParam(FFloatSetterTemplate, float, Param);

	/**
	 * float 型の選択項目の Getter 関数を指定するためのデリゲート
	 */
	DECLARE_DYNAMIC_DELEGATE_OneParam(FFloatDynamicOptionsTemplate, TArray<float>&, OutParam);

	/**
	 * FString 型 Getter 関数を指定するためのデリゲート
	 */
	DECLARE_DYNAMIC_DELEGATE_RetVal(FString, FStringGetterTemplate);

	/**
	 * FString 型 Setter 関数を指定するためのデリゲート
	 */
	DECLARE_DYNAMIC_DELEGATE_OneParam(FStringSetterTemplate, FString, Param);

	/**
	 * FString 型の選択項目の Getter 関数を指定するためのデリゲート
	 */
	DECLARE_DYNAMIC_DELEGATE_OneParam(FStringDynamicOptionsTemplate, TArray<FString>&, OutParam);

};


/**
 * 選択タイプ設定項目に対する必要情報定義用のフォーマットのベースクラス
 */
UCLASS(Abstract, meta = (DisplayName = "FormatBase Discrete"))
class GSCORE_API UGSCDiscretePropertyFormatBase : public UGSCPropertyFormatBase
{
public:
	GENERATED_BODY()

protected:
	//
	// 選択項目の表示名をオーバーライドするためのマッピングリスト
	// 
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI Parameter", meta = (GetOptions = "GetOptionStrings"))
	TMap<FString, FText> OptionNameOverrides;


public:
	/**
	 * 設定の DynamicOptionSource を取得する
	 */
	virtual const FGSCPicker_PropertySource& GetDynamicOptionSource() const { return FGSCPicker_PropertySource::Empty; }


public:
	/**
	 * この Format によって定義されたプロパティ設定項目の選択項目を変更する
	 */
	virtual bool SetPropertyOption(int32 NewOptionIdx) { return false; }

	/**
	 * この Format によって定義されたプロパティ設定項目の選択項目を取得する
	 */
	virtual bool GetPropertyOption(int32& OutOptionIdx) { return false; }

	/**
	 * この Format によって定義されたプロパティ設定項目の選択項目を取得する
	 */
	virtual bool GetPropertyDefaultOption(int32& OutOptionIdx) { return false; }

	/**
	 * この Format によって定義された選択タイプのプロパティ設定項目の選択項目の名前を返す
	 */
	virtual TArray<FText> GetOptionNames() const;


protected:
	/**
	 * DataSource から選択項目を取得可能かどうかを返す
	 */
	virtual bool CanUseDynamicOptionSource() const;

	/**
	 * DataSource から選択項目を FString 型 で取得する
	 */
	virtual bool GetDynamicOptionStrings(TArray<FString>& OutOptions) const { return false; }

	/**
	 * 選択項目を FString 型 で取得する
	 */
	UFUNCTION()
	virtual TArray<FString> GetOptionStrings() const { return TArray<FString>(); }

};


/**
 * int 型の選択タイプの設定項目に対する必要情報定義用のフォーマットクラス
 */
UCLASS(meta = (DisplayName = "Format Discrete Integer"))
class GSCORE_API UGSCFormat_DiscreteInt : public UGSCDiscretePropertyFormatBase
{
public:
	GENERATED_BODY()

protected:
	//
	// 設定の値の Getter 関数名
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DataSource", meta = (
		GetterFuncTemplate = "/Script/GSCore.GSCPickerTemplate_FormatDiscrete::IntGetterTemplate__DelegateSignature"))
	FGSCPicker_PropertySource GetterSource{ FGSCPicker_PropertySource::Empty };

	//
	// 設定の値の Setter 関数名
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DataSource", meta = (
		SetterFuncTemplate = "/Script/GSCore.GSCPickerTemplate_FormatDiscrete::IntSetterTemplate__DelegateSignature"))
	FGSCPicker_PropertySource SetterSource{ FGSCPicker_PropertySource::Empty };

	//
	// 設定の選択項目の Getter 関数名
	// 
	// Tips:
	//  設定しない場合はフォーマットで設定したオプションが使用される
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DataSource", meta = (
		SetterFuncTemplate = "/Script/GSCore.GSCPickerTemplate_FormatDiscrete::IntDynamicOptionsTemplate__DelegateSignature"))
	FGSCPicker_PropertySource DynamicOptionSource{ FGSCPicker_PropertySource::Empty };


public:
	//
	// 設定値と表示名のマッピングリスト
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Options")
	TMap<FString, int32> Options;


public:
	virtual const FGSCPicker_PropertySource& GetGetterSource() const override;
	virtual const FGSCPicker_PropertySource& GetSetterSource() const override;
	virtual const FGSCPicker_PropertySource& GetDefaultSource() const override;
	virtual const FGSCPicker_PropertySource& GetDynamicOptionSource() const override;

public:
	virtual bool SetPropertyOption(int32 NewOptionIdx) override;
	virtual bool GetPropertyOption(int32& OutOptionIdx) override;
	virtual bool GetPropertyDefaultOption(int32& OutOptionIdx) override;

protected:
	virtual bool GetDynamicOptionStrings(TArray<FString>& OutOptions) const override;
	virtual TArray<FString> GetOptionStrings() const override;

	/**
	 * DataSource から選択項目を int型で取得する
	 */
	virtual bool GetDynamicIntOptions(TArray<int32>& OutOptions) const;
};


/**
 * float 型の選択タイプの設定項目に対する必要情報定義用のフォーマットクラス
 */
UCLASS(meta = (DisplayName = "Format Discrete Float"))
class GSCORE_API UGSCFormat_DiscreteFloat : public UGSCDiscretePropertyFormatBase
{
public:
	GENERATED_BODY()

protected:
	//
	// 設定の値の Getter 関数名
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DataSource", meta = (
		GetterFuncTemplate = "/Script/GSCore.GSCPickerTemplate_FormatDiscrete::FloatGetterTemplate__DelegateSignature"))
	FGSCPicker_PropertySource GetterSource{ FGSCPicker_PropertySource::Empty };

	//
	// 設定の値の Setter 関数名
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DataSource", meta = (
		SetterFuncTemplate = "/Script/GSCore.GSCPickerTemplate_FormatDiscrete::FloatSetterTemplate__DelegateSignature"))
	FGSCPicker_PropertySource SetterSource{ FGSCPicker_PropertySource::Empty };

	//
	// 設定の選択項目の Getter 関数名
	// 
	// Tips:
	//  設定しない場合はフォーマットで設定したオプションが使用される
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DataSource", meta = (
		SetterFuncTemplate = "/Script/GSCore.GSCPickerTemplate_FormatDiscrete::FloatDynamicOptionsTemplate__DelegateSignature"))
	FGSCPicker_PropertySource DynamicOptionSource{ FGSCPicker_PropertySource::Empty };


public:
	//
	// 設定値と表示名のマッピングリスト
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Options")
	TMap<FString, float> Options;


public:
	virtual const FGSCPicker_PropertySource& GetGetterSource() const override;
	virtual const FGSCPicker_PropertySource& GetSetterSource() const override;
	virtual const FGSCPicker_PropertySource& GetDefaultSource() const override;
	virtual const FGSCPicker_PropertySource& GetDynamicOptionSource() const override;

public:
	virtual bool SetPropertyOption(int32 NewOptionIdx) override;
	virtual bool GetPropertyOption(int32& OutOptionIdx) override;
	virtual bool GetPropertyDefaultOption(int32& OutOptionIdx) override;

protected:
	virtual bool GetDynamicOptionStrings(TArray<FString>& OutOptions) const override;
	virtual TArray<FString> GetOptionStrings() const override;

	/**
	 * DataSource から選択項目を float型で取得する
	 */
	virtual bool GetDynamicFloatOptions(TArray<float>& OutOptions) const;
};


/**
 * FString 型の選択タイプの設定項目に対する必要情報定義用のフォーマットクラス
 */
UCLASS(meta = (DisplayName = "Format Discrete String"))
class GSCORE_API UGSCFormat_DiscreteString : public UGSCDiscretePropertyFormatBase
{
public:
	GENERATED_BODY()

protected:
	//
	// 設定の値の Getter 関数名
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DataSource", meta = (
		GetterFuncTemplate = "/Script/GSCore.GSCPickerTemplate_FormatDiscrete::StringGetterTemplate__DelegateSignature"))
	FGSCPicker_PropertySource GetterSource{ FGSCPicker_PropertySource::Empty };

	//
	// 設定の値の Setter 関数名
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DataSource", meta = (
		SetterFuncTemplate = "/Script/GSCore.GSCPickerTemplate_FormatDiscrete::StringSetterTemplate__DelegateSignature"))
	FGSCPicker_PropertySource SetterSource{ FGSCPicker_PropertySource::Empty };

	//
	// 設定の選択項目の Getter 関数名
	// 
	// Tips:
	//  設定しない場合はフォーマットで設定したオプションが使用される
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DataSource", meta = (
		SetterFuncTemplate = "/Script/GSCore.GSCPickerTemplate_FormatDiscrete::StringDynamicOptionsTemplate__DelegateSignature"))
	FGSCPicker_PropertySource DynamicOptionSource{ FGSCPicker_PropertySource::Empty };


public:
	//
	// 設定値と表示名のマッピングリスト
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Options")
	TArray<FString> Options;


public:
	virtual const FGSCPicker_PropertySource& GetGetterSource() const override;
	virtual const FGSCPicker_PropertySource& GetSetterSource() const override;
	virtual const FGSCPicker_PropertySource& GetDefaultSource() const override;
	virtual const FGSCPicker_PropertySource& GetDynamicOptionSource() const override;

public:
	virtual bool SetPropertyOption(int32 NewOptionIdx) override;
	virtual bool GetPropertyOption(int32& OutOptionIdx) override;
	virtual bool GetPropertyDefaultOption(int32& OutOptionIdx) override;

protected:
	virtual bool GetDynamicOptionStrings(TArray<FString>& OutOptions) const override;
	virtual TArray<FString> GetOptionStrings() const override;

};
