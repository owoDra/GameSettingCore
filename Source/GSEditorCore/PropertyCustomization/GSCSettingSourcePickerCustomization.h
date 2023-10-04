// Copyright (C) 2023 owoDra

#pragma once

#include "PropertyCustomization/GSCSourcePickerCustomizationBase.h"

class FPropertyEditorModule;


/**
 * FGSCPicker_SettingSource 型のプロパティを編集する際に選択した SourceClass に応じて
 * 自動的に使用可能な関数名の候補が選択されるようにするためのクラス
 */
class FGSCSettingSourcePickerCustomization : public FGSCSourcePickerCustomizationBase
{
private:
	typedef FGSCSourcePickerCustomizationBase Super;
	typedef FGSCSettingSourcePickerCustomization ThisClass;

	static const FName NAME_SettingSourcePickerProperty;

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
	virtual FName GetSourceClassPropertyName() const override;
	virtual FName GetFunctionNamePropertyName() const override;
	virtual bool ShouldStaticFunction() const override;

	virtual bool UpdateTemplateFunction() override;
	virtual const UClass* GetSelectedSourceClass() const override;

};
