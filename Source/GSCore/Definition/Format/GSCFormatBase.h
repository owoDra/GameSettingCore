// Copyright (C) 2023 owoDra

#pragma once

#include "UObject/Object.h"

#include "Definition/Picker/GSCPicker_PropertySource.h"

#include "GSCFormatBase.generated.h"

class UGSCEditCondition;


/**
 * EditCondition の判定の仕方を定義する列挙型
 */
UENUM(BlueprintType)
enum class EGSCEditConditionType : uint8
{
	// 無条件で常に編集可能な設定

	AlwaysAllowed	UMETA(DisplayName = "Always Allowed"),

	// 指定したデータソースの値に基づいて編集可能か決める設定

	ByDataSource	UMETA(DisplayName = "Determined By Data Source"),

	// 指定したフラグメントの値に基づいて編集可能か決める設定

	ByFragment		UMETA(DisplayName = "Determined By Fragment"),
};


/**
 * エディターで関数の選択を可能にする際に関数を絞り込むための型定義のみ行ったダミーのデリゲートのラッパー
 *  
 * Tips:
 *  ここで定義されたデリゲートは主に UGSCFormatBase 内でのデータソースの設定に使用される。
 */
UCLASS(Abstract, Const, Transient)
class UGSCPickerTemplate_FormatBase : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * 設定へアクセス可能なオブジェクトを取得する関数の型を指定するためのデリゲート
	 */
	DECLARE_DYNAMIC_DELEGATE_RetVal(UObject*, FStaticContextTemplate);

	/**
	 * 設定が変更可能かどうかを取得する関数の型を指定するためのデリゲート
	 */
	DECLARE_DYNAMIC_DELEGATE_RetVal(bool, FEditConditionTemplate);

};


/**
 * 設定項目の定義に使用可能な型を定義するクラス
 * このクラスから派生することで様々なデータ型に対応した設定項目の定義が可能となる
 */
UCLASS(Abstract, DefaultToInstanced, EditInlineNew, NotBlueprintable)
class GSCORE_API UGSCFormatBase : public UObject
{
public:
	GENERATED_BODY()

protected:
	//
	// EditCondition の判定の仕方
	// 
	// AlwaysAllowed	: 無条件で常に編集可能な設定
	// ByDataSource		: 指定したデータソースの値に基づいて編集可能か決める設定
	// ByFragment		: 指定したフラグメントの値に基づいて編集可能か決める設定
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EditCondition")
	EGSCEditConditionType EditConditionType{ EGSCEditConditionType::AlwaysAllowed };

	//
	// 設定が変更可能かどうかを取得する関数の名前
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EditCondition", meta = (
		EditCondition = "EditConditionType == EGSCEditConditionType::ByDataSource", EditConditionHides,
		GetterFuncTemplate = "/Script/GSCore.GSCPickerTemplate_FormatBase::EditConditionTemplate__DelegateSignature"))
	FGSCPicker_PropertySource EditConditionSource{ FGSCPicker_PropertySource::Empty };

	//
	// 設定が変更可能かどうかを取得するフラグメント
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced, Category = "EditCondition", meta = (
		EditCondition = "EditConditionType == EGSCEditConditionType::ByFragment", EditConditionHides))
	TObjectPtr<const UGSCEditCondition> EditConditionFragment{ nullptr };


public:
	/**
	 * 設定の GetterSource を取得する
	 */
	virtual FGSCPicker_PropertySource GetGetterSource() const { return FGSCPicker_PropertySource::Empty; }

	/**
	 * 設定の SetterSource を取得する
	 */
	virtual FGSCPicker_PropertySource GetSetterSource() const { return FGSCPicker_PropertySource::Empty; }

};
