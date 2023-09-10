// Copyright (C) 2023 owoDra

#pragma once

#include "IPropertyTypeCustomization.h"

class UObject;
class STextBlock;
class SSearchableComboBox;


/**
 * SourceClass を選びそれに対応する関数を選択するタイプの構造体のエディターでの編集を
 * カスタマイズするためのベースクラス
 * 
 * 想定される構造体のフォーマット:
 *  - SourceClass  : 特別な型指定はないがクラス名を FString 型として取得可能な物 (UClass*, FSoftClassPath など)
 *  - FunctionName : 必ず FName 型である必要がある
 */
class FGSCSourcePickerCustomizationBase : public IPropertyTypeCustomization
{
private:
	typedef FGSCSourcePickerCustomizationBase ThisClass;

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
	// Picker 構造体の SourceClass 変数へのハンドル
	//
	TSharedPtr<IPropertyHandle> SourceClassPropertyHandle{ nullptr };

	//
	// Picker 構造体の FunctionName 変数へのハンドル
	//
	TSharedPtr<IPropertyHandle> FunctionNamePropertyHandle{ nullptr };

	//
	// 前回選択した SourceClass の名前
	//
	FString PrevSelectedSourceCalssName{ FString() };

	//
	// 関数名の候補をフィルターするためのクラス
	//
	TWeakObjectPtr<UFunction> TemplateFunctionWeakPtr{ nullptr };

protected:
	/**
	 * SourceClass のプロパティ名を返す
	 */
	virtual FName GetSourceClassPropertyName() const { return NAME_None; }

	/**
	 * FunctionName のプロパティ名を返す
	 */
	virtual FName GetFunctionNamePropertyName() const { return NAME_None; }

	/**
	 * 関数名の候補に static 関数を表示するかどうかを返す
	 */
	virtual bool ShouldStaticFunction() const { return false; }


public:
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;

protected:
	/**
	 * SourceClass プロパティに対する編集ウィジェットのカスタマイズを行う
	 * 
	 * Tips:
	 *  この関数は SourceClassPropertyHandle.IsValid() を確認してから呼び出します
	 */
	virtual void CustomizeSourceClassProperty(IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils);

	/**
	 * FunctionName プロパティに対する編集ウィジェットのカスタマイズを行う
	 * 
	 * Tips:
	 *  この関数は FunctionNamePropertyHandle.IsValid() を確認してから呼び出します
	 */
	virtual void CustomizeFunctionNameProperty(IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils);


protected:
	/**
	 * プロパティの値が変更されたときに実行
	 */
	virtual void HandlePropertyChange();


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


protected:
	/**
	 * 変数の Meta指定子 から現在の TemplateFunction を更新する
	 */
	virtual bool UpdateTemplateFunction() { return true; }

	/**
	 * 現在 SourceClass で選択しているクラスを取得する
	 */
	virtual const UClass* GetSelectedSourceClass() const;

	/**
	 * 指定した関数が TemplateFunction で指定された署名と一致するかどうかを返す
	 */
	virtual bool IsSignatureCompatible(const UFunction* Function) const;

};
