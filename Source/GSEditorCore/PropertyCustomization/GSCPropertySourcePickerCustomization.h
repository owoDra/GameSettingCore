// Copyright (C) 2023 owoDra

#pragma once

#include "PropertyCustomization/GSCSourcePickerCustomizationBase.h"

class UObject;
class STextBlock;
class SSearchableComboBox;
class FPropertyEditorModule;


/**
 * �f�[�^�\�[�X�̑I���Ŏ����I�Ɏg�p�\�Ȋ֐��̌�₪�I�������悤�ɂ��邽�߂̃N���X
 */
class FGSCPropertySourcePickerCustomization : public FGSCSourcePickerCustomizationBase
{
private:
	typedef FGSCSourcePickerCustomizationBase Super;
	typedef FGSCPropertySourcePickerCustomization ThisClass;

	static const FName NAME_PropertySourcePickerProperty;

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
	//
	// ���ݑI��ł��� SettingSource �̖��O��\�����邽�߂̃X���[�g�E�B�W�F�b�g
	//
	TWeakPtr<STextBlock> SettingSourceTextWidget{ nullptr };

	//
	// �I���\�� SettingSource �̖��O��\�����邽�߂̃X���[�g�E�B�W�F�b�g
	//
	TWeakPtr<SSearchableComboBox> SuggestSettingSourcesWidget{ nullptr };

	//
	// SettingSource �p�� CombBox ���̃I�v�V�������X�g
	//
	TArray<TSharedPtr<FString>> SuggestSettingSourceOptions;

	//
	// SettingSource �I�����Ȃ��ꍇ�̃I�v�V������
	//
	TWeakPtr<FString> NoneSettingSourceOption{ nullptr };

	//
	// ���ݕҏW���� Picker �ϐ��� Meta�w��q �̃L�[
	//
	FName TemplateMetaKeyName{ NAME_None };

	//
	// ���ݕҏW���� Picker �ϐ��� Meta�w��q �̒l
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
	 * �I�𒆂� SettingSource ����ݒ肷��
	 */
	void SetSettingSourceNameValue(FName Value);

	/**
	 * CombBox �ɋ�̍��ڂ���x�ǉ��� ���[�U�[ �̑I������������
	 */
	void InitSuggestSettingSources();

	/**
	 * CombBox �������ׂč폜�� None �����̏�Ԃɂ���
	 */
	void ResetSuggestSettingSources();

	/**
	 * �ϐ��ɕύX���������Ƃ��ɌĂяo��
	 * �f�x���b�p�[�ݒ�̒l�ɉ����� SuggestSettingSourceOptions �̒l���X�V����
	 */
	void RefreshSuggestSettingSources();

	/**
	 * CombBox ���� SettingSources ���̌���I�������Ƃ��ɌĂяo�����
	 * ���݂� SettingSourcesName �̒l���X�V����
	 */
	void OnSuggestSettingSourceSelected(TSharedPtr<FString> SelectedStringPtr, ESelectInfo::Type SelectInfo);


protected:
	virtual bool UpdateTemplateFunction() override;
	virtual const UClass* GetSelectedSourceClass() const override;

	/**
	 * Meta�w��q �̃L�[�ƒl���L�^����
	 */
	void CacheTemplateMetaKey();
};
