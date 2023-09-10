// Copyright (C) 2023 owoDra

#pragma once

#include "IPropertyTypeCustomization.h"

class UObject;
class STextBlock;
class SSearchableComboBox;


/**
 * SourceClass ��I�т���ɑΉ�����֐���I������^�C�v�̍\���̂̃G�f�B�^�[�ł̕ҏW��
 * �J�X�^�}�C�Y���邽�߂̃x�[�X�N���X
 * 
 * �z�肳���\���̂̃t�H�[�}�b�g:
 *  - SourceClass  : ���ʂȌ^�w��͂Ȃ����N���X���� FString �^�Ƃ��Ď擾�\�ȕ� (UClass*, FSoftClassPath �Ȃ�)
 *  - FunctionName : �K�� FName �^�ł���K�v������
 */
class FGSCSourcePickerCustomizationBase : public IPropertyTypeCustomization
{
private:
	typedef FGSCSourcePickerCustomizationBase ThisClass;

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
	// Picker �\���̂� SourceClass �ϐ��ւ̃n���h��
	//
	TSharedPtr<IPropertyHandle> SourceClassPropertyHandle{ nullptr };

	//
	// Picker �\���̂� FunctionName �ϐ��ւ̃n���h��
	//
	TSharedPtr<IPropertyHandle> FunctionNamePropertyHandle{ nullptr };

	//
	// �O��I������ SourceClass �̖��O
	//
	FString PrevSelectedSourceCalssName{ FString() };

	//
	// �֐����̌����t�B���^�[���邽�߂̃N���X
	//
	TWeakObjectPtr<UFunction> TemplateFunctionWeakPtr{ nullptr };

protected:
	/**
	 * SourceClass �̃v���p�e�B����Ԃ�
	 */
	virtual FName GetSourceClassPropertyName() const { return NAME_None; }

	/**
	 * FunctionName �̃v���p�e�B����Ԃ�
	 */
	virtual FName GetFunctionNamePropertyName() const { return NAME_None; }

	/**
	 * �֐����̌��� static �֐���\�����邩�ǂ�����Ԃ�
	 */
	virtual bool ShouldStaticFunction() const { return false; }


public:
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;

protected:
	/**
	 * SourceClass �v���p�e�B�ɑ΂���ҏW�E�B�W�F�b�g�̃J�X�^�}�C�Y���s��
	 * 
	 * Tips:
	 *  ���̊֐��� SourceClassPropertyHandle.IsValid() ���m�F���Ă���Ăяo���܂�
	 */
	virtual void CustomizeSourceClassProperty(IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils);

	/**
	 * FunctionName �v���p�e�B�ɑ΂���ҏW�E�B�W�F�b�g�̃J�X�^�}�C�Y���s��
	 * 
	 * Tips:
	 *  ���̊֐��� FunctionNamePropertyHandle.IsValid() ���m�F���Ă���Ăяo���܂�
	 */
	virtual void CustomizeFunctionNameProperty(IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils);


protected:
	/**
	 * �v���p�e�B�̒l���ύX���ꂽ�Ƃ��Ɏ��s
	 */
	virtual void HandlePropertyChange();


protected:
	/**
	 * �I�𒆂̊֐�����ݒ肷��
	 */
	void SetFunctionNameValue(FName Value);

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


protected:
	/**
	 * �ϐ��� Meta�w��q ���猻�݂� TemplateFunction ���X�V����
	 */
	virtual bool UpdateTemplateFunction() { return true; }

	/**
	 * ���� SourceClass �őI�����Ă���N���X���擾����
	 */
	virtual const UClass* GetSelectedSourceClass() const;

	/**
	 * �w�肵���֐��� TemplateFunction �Ŏw�肳�ꂽ�����ƈ�v���邩�ǂ�����Ԃ�
	 */
	virtual bool IsSignatureCompatible(const UFunction* Function) const;

};
