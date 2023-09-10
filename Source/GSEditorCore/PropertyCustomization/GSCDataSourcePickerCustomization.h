// Copyright (C) 2023 owoDra

#pragma once

#include "IPropertyTypeCustomization.h"

class UObject;
class STextBlock;
class SSearchableComboBox;


/**
 * データソースの選択で自動的に使用可能な関数の候補が選択されるようにするためのクラス
 */
class FGSCDataSourcePickerCustomization : public IPropertyTypeCustomization
{
	typedef FGSCDataSourcePickerCustomization ThisClass;

protected:
	//
	// 現在選んでいる関数を表示するためのスレートウィジェット
	//
	TWeakPtr<STextBlock> RowTextWidget{ nullptr };

	//
	// 選択可能な関数を表示するためのスレートウィジェット
	//
	TWeakPtr<SSearchableComboBox> SuggestListWidget{ nullptr };

	//
	// CombBox 内のオプションリスト
	//
	TArray<TSharedPtr<FString>> SuggestListOptions;

	//
	// クラスを選択しない場合のオプション名
	//
	TWeakPtr<FString> NoneStringOption{ nullptr };

	//
	// 編集する変数を所有しているオブジェクト
	//
	TWeakObjectPtr<UObject> PropertyOuterObject{nullptr};

	//
	// 編集する Picker 変数へのハンドル
	//
	TSharedPtr<IPropertyHandle> ParentPropertyHandle{ nullptr };

	//
	// Picker 構造体の SourceClass 変数へのハンドル
	//
	TSharedPtr<IPropertyHandle> SourceClassHandle{ nullptr };

	//
	// Picker 構造体の FunctionName 変数へのハンドル
	//
	TSharedPtr<IPropertyHandle> FunctionNameHandle{ nullptr };

	//
	// 前回選択した SourceClass の名前
	//
	FName PrevSelectedSourceCalssName{ NAME_None };

	//
	// 関数名の候補をフィルターするためのクラス
	//
	TWeakObjectPtr<UFunction> TemplateFunctionWeakPtr{ nullptr };

	//
	// 現在編集中の Picker 変数の Meta指定子 のキー
	//
	FName TemplateMetaKeyName{ NAME_None };

	//
	// 現在編集中の Picker 変数の Meta指定子 の値
	//
	FName TemplateMetaValueName{ NAME_None };

	//
	// 関数名の候補に static 関数を表示するかどうか
	//
	bool bIsStaticFunctionInternal{ false };

public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;


protected:
	/**
	 * 選択中の関数名を設定する
	 */
	void SetFunctionNameValue(FName Value);

	/**
	 * CombBox に空の項目を一度追加し ユーザー の選択を解除する
	 */
	void InitSuggestList();

	/**
	 * CombBox 内をすべて削除し None だけの状態にする
	 */
	void ResetSuggestList();

	/**
	 * 変数に変更があったときに呼び出す
	 * 現在の状態をもとに関数名の候補をフィルターし SearchableComboBoxValues の値を更新する
	 */
	void RefreshSuggestList();

	/**
	 * SourceClass が無効な参照をしたときに呼び出し候補リストを無効化する
	 */
	void InvalidateSuggestList();

	/**
	 * 候補リストを無効にする
	 */
	void SetSuggestListEnabled(bool bEnabled);

	/**
	 * CombBox から関数名の候補を選択したときに呼び出される
	 * 現在の FunctionName の値を更新する
	 */
	void OnSuggestSelected(TSharedPtr<FString> SelectedStringPtr, ESelectInfo::Type SelectInfo);


protected:
	/**
	 * 現在 SourceClass で選択しているクラスを取得する
	 */
	const UClass* GetSelectedSourceClass() const;

	/**
	 * 指定した関数が TemplateFunction で指定された署名と一致するかどうかを返す
	 */
	bool IsSignatureCompatible(const UFunction* Function) const;

	/**
	 * 変数の Meta指定子 から現在の TemplateFunction を更新する
	 */
	bool UpdateTemplateFunction();

	/**
	 * Meta指定子 のキーと値を記録する
	 */
	void CacheTemplateMetaKey();
};
