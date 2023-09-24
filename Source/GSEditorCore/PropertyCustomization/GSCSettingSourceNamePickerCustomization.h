// Copyright (C) 2023 owoDra

#pragma once

#include "IPropertyTypeCustomization.h"

class UObject;
class STextBlock;
class SSearchableComboBox;


/**
 * Developer Settings �œo�^���� SettingSource �̖��O��I������^�C�v�̍\���̂̃G�f�B�^�[�ł̕ҏW��
 * �J�X�^�}�C�Y���邽�߂̃x�[�X�N���X
 */
class FGSCSettingSourceNamePickerCustomization : public IPropertyTypeCustomization
{
private:
	typedef FGSCSettingSourceNamePickerCustomization ThisClass;

	static const FName NAME_SettingSourceNamePickerProperty;

protected:
	//
	// ���ݑI��ł���֐���\�����邽�߂̃X���[�g�E�B�W�F�b�g
	//
	TWeakPtr<STextBlock> RowTextWidget{ nullptr };

	//
	// �I���\�Ȋ֐���\�����邽�߂̃X���[�g�E�B�W�F�b�g
	//
	TWeakPtr<SSearchableComboBox> SuggestListWidget{ nullptr };

	//
	// CombBox ���̃I�v�V�������X�g
	//
	TArray<TSharedPtr<FString>> SuggestListOptions;

	//
	// �N���X��I�����Ȃ��ꍇ�̃I�v�V������
	//
	TWeakPtr<FString> NoneStringOption{ nullptr };

	//
	// �ҏW���� Picker �ϐ��ւ̃n���h��
	//
	TSharedPtr<IPropertyHandle> ParentPropertyHandle{ nullptr };

	//
	// Picker �\���̂� Name �ϐ��ւ̃n���h��
	//
	TSharedPtr<IPropertyHandle> SourceNamePropertyHandle{ nullptr };

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


public:
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;


protected:
	/**
	 * �I�𒆂� SettingSource ����ݒ肷��
	 */
	void SetSettingSourceNameValue(FName Value);

	/**
	 * CombBox �ɋ�̍��ڂ���x�ǉ��� ���[�U�[ �̑I������������
	 */
	void InitSuggestList();

	/**
	 * CombBox �������ׂč폜�� None �����̏�Ԃɂ���
	 */
	void ResetSuggestList();

	/**
	 * �ϐ��ɕύX���������Ƃ��ɌĂяo��
	 * ���݂̏�Ԃ����ƂɊ֐����̌����t�B���^�[�� SuggestListOptions �̒l���X�V����
	 */
	void RefreshSuggestList();

	/**
	 * SourceClass �������ȎQ�Ƃ������Ƃ��ɌĂяo����⃊�X�g�𖳌�������
	 */
	void InvalidateSuggestList();

	/**
	 * CombBox ����֐����̌���I�������Ƃ��ɌĂяo�����
	 * ���݂� FunctionName �̒l���X�V����
	 */
	void OnSuggestSelected(TSharedPtr<FString> SelectedStringPtr, ESelectInfo::Type SelectInfo);

};
