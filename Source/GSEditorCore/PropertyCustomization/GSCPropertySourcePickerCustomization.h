// Copyright (C) 2023 owoDra

#pragma once

#include "PropertyCustomization/GSCSourcePickerCustomizationBase.h"

class UObject;
class STextBlock;
class SSearchableComboBox;
class FPropertyEditorModule;


/**
 * データソースの選択で自動的に使用可能な関数の候補が選択されるようにするためのクラス
 */
class FGSCPropertySourcePickerCustomization : public FGSCSourcePickerCustomizationBase
{
private:
	typedef FGSCSourcePickerCustomizationBase Super;
	typedef FGSCPropertySourcePickerCustomization ThisClass;

	static const FName NAME_PropertySourcePickerProperty;

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


protected:
	//
	// 現在選んでいる SettingSource の名前を表示するためのスレートウィジェット
	//
	TWeakPtr<STextBlock> SettingSourceTextWidget{ nullptr };

	//
	// 選択可能な SettingSource の名前を表示するためのスレートウィジェット
	//
	TWeakPtr<SSearchableComboBox> SuggestSettingSourcesWidget{ nullptr };

	//
	// SettingSource 用の CombBox 内のオプションリスト
	//
	TArray<TSharedPtr<FString>> SuggestSettingSourceOptions;

	//
	// SettingSource 選択しない場合のオプション名
	//
	TWeakPtr<FString> NoneSettingSourceOption{ nullptr };

	//
	// 現在編集中の Picker 変数の Meta指定子 のキー
	//
	FName TemplateMetaKeyName{ NAME_None };

	//
	// 現在編集中の Picker 変数の Meta指定子 の値
	//
	FName TemplateMetaValueName{ NAME_None };

protected:
	virtual FName GetSourceClassPropertyName() const override;
	virtual FName GetFunctionNamePropertyName() const override;
	virtual bool ShouldStaticFunction() const override;


protected:
	virtual void CustomizeSourceClassProperty(IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeFunctionNameProperty(IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;


protected:
	/**
	 * 選択中の SettingSource 名を設定する
	 */
	void SetSettingSourceNameValue(FName Value);

	/**
	 * CombBox に空の項目を一度追加し ユーザー の選択を解除する
	 */
	void InitSuggestSettingSources();

	/**
	 * CombBox 内をすべて削除し None だけの状態にする
	 */
	void ResetSuggestSettingSources();

	/**
	 * 変数に変更があったときに呼び出す
	 * デベロッパー設定の値に応じて SuggestSettingSourceOptions の値を更新する
	 */
	void RefreshSuggestSettingSources();

	/**
	 * CombBox から SettingSources 名の候補を選択したときに呼び出される
	 * 現在の SettingSourcesName の値を更新する
	 */
	void OnSuggestSettingSourceSelected(TSharedPtr<FString> SelectedStringPtr, ESelectInfo::Type SelectInfo);


protected:
	virtual bool UpdateTemplateFunction() override;
	virtual const UClass* GetSelectedSourceClass() const override;

	/**
	 * Meta指定子 のキーと値を記録する
	 */
	void CacheTemplateMetaKey();
};
