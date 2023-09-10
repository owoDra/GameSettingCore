// Copyright (C) 2023 owoDra

#pragma once

#include "PropertyCustomization/GSCSourcePickerCustomizationBase.h"

class FPropertyEditorModule;


/**
 * FGSCPicker_SettingSource �^�̃v���p�e�B��ҏW����ۂɑI������ SourceClass �ɉ�����
 * �����I�Ɏg�p�\�Ȋ֐����̌�₪�I�������悤�ɂ��邽�߂̃N���X
 */
class FGSCSettingSourcePickerCustomization : public FGSCSourcePickerCustomizationBase
{
private:
	typedef FGSCSourcePickerCustomizationBase Super;
	typedef FGSCSettingSourcePickerCustomization ThisClass;

	static const FName NAME_SettingSourcePickerProperty;

public:
	/**
	 * ���̃N���X�̃C���X�^���X���쐬����
	 */
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	/**
	 * ���̃N���X�����W���[���ɓo�^����
	 */
	static void RegisterToPropertyEditorModule(FPropertyEditorModule& PropertyModule);

	/**
	 * ���̃N���X�����W���[������o�^��������
	 */
	static void UnregisterToPropertyEditorModule(FPropertyEditorModule& PropertyModule);


protected:
	virtual FName GetSourceClassPropertyName() const override;
	virtual FName GetFunctionNamePropertyName() const override;
	virtual bool ShouldStaticFunction() const override;

	virtual bool UpdateTemplateFunction() override;
	virtual const UClass* GetSelectedSourceClass() const override;

};
