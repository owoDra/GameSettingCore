// Copyright (C) 2023 owoDra

#pragma once

#include "IPropertyTypeCustomization.h"

class UObject;
class STextBlock;
class SSearchableComboBox;


/**
 * Developer Settings で登録した SettingSource の名前を選択するタイプの構造体のエディターでの編集を
 * カスタマイズするためのベースクラス
 */
class FGSCSettingSourceNamePickerCustomization : public IPropertyTypeCustomization
{
private:
	typedef FGSCSettingSourceNamePickerCustomization ThisClass;

	static const FName NAME_SettingSourceNamePickerProperty;

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
	// 編集する Picker 変数へのハンドル
	//
	TSharedPtr<IPropertyHandle> ParentPropertyHandle{ nullptr };

	//
	// Picker 構造体の Name 変数へのハンドル
	//
	TSharedPtr<IPropertyHandle> SourceNamePropertyHandle{ nullptr };

public:
	/**
	 * このクラスのインスタンスを作成する
	 */
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	/**
	 * このクラスをモジュールに登録する
	 */
	static void RegisterToPropertyEditorModule(FPropertyEditorModule& PropertyModule);

	/**
	 * このクラスをモジュールから登録解除する
	 */
	static void UnregisterToPropertyEditorModule(FPropertyEditorModule& PropertyModule);


public:
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;


protected:
	/**
	 * 選択中の SettingSource 名を設定する
	 */
	void SetSettingSourceNameValue(FName Value);

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
	 * 現在の状態をもとに関数名の候補をフィルターし SuggestListOptions の値を更新する
	 */
	void RefreshSuggestList();

	/**
	 * SourceClass が無効な参照をしたときに呼び出し候補リストを無効化する
	 */
	void InvalidateSuggestList();

	/**
	 * CombBox から関数名の候補を選択したときに呼び出される
	 * 現在の FunctionName の値を更新する
	 */
	void OnSuggestSelected(TSharedPtr<FString> SelectedStringPtr, ESelectInfo::Type SelectInfo);

};
