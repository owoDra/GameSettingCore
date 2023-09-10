// Copyright (C) 2023 owoDra

#pragma once

#include "IPropertyTypeCustomization.h"

class UObject;
class STextBlock;
class SSearchableComboBox;


/**
 * �f�[�^�\�[�X�̑I���Ŏ����I�Ɏg�p�\�Ȋ֐��̌�₪�I�������悤�ɂ��邽�߂̃N���X
 */
class FGSCDataSourcePickerCustomization : public IPropertyTypeCustomization
{
	typedef FGSCDataSourcePickerCustomization ThisClass;

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
	// �ҏW����ϐ������L���Ă���I�u�W�F�N�g
	//
	TWeakObjectPtr<UObject> PropertyOuterObject{nullptr};

	//
	// �ҏW���� Picker �ϐ��ւ̃n���h��
	//
	TSharedPtr<IPropertyHandle> ParentPropertyHandle{ nullptr };

	//
	// Picker �\���̂� SourceClass �ϐ��ւ̃n���h��
	//
	TSharedPtr<IPropertyHandle> SourceClassHandle{ nullptr };

	//
	// Picker �\���̂� FunctionName �ϐ��ւ̃n���h��
	//
	TSharedPtr<IPropertyHandle> FunctionNameHandle{ nullptr };

	//
	// �O��I������ SourceClass �̖��O
	//
	FName PrevSelectedSourceCalssName{ NAME_None };

	//
	// �֐����̌����t�B���^�[���邽�߂̃N���X
	//
	TWeakObjectPtr<UFunction> TemplateFunctionWeakPtr{ nullptr };

	//
	// ���ݕҏW���� Picker �ϐ��� Meta�w��q �̃L�[
	//
	FName TemplateMetaKeyName{ NAME_None };

	//
	// ���ݕҏW���� Picker �ϐ��� Meta�w��q �̒l
	//
	FName TemplateMetaValueName{ NAME_None };

	//
	// �֐����̌��� static �֐���\�����邩�ǂ���
	//
	bool bIsStaticFunctionInternal{ false };

public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;


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
	 * ���݂̏�Ԃ����ƂɊ֐����̌����t�B���^�[�� SearchableComboBoxValues �̒l���X�V����
	 */
	void RefreshSuggestList();

	/**
	 * SourceClass �������ȎQ�Ƃ������Ƃ��ɌĂяo����⃊�X�g�𖳌�������
	 */
	void InvalidateSuggestList();

	/**
	 * ��⃊�X�g�𖳌��ɂ���
	 */
	void SetSuggestListEnabled(bool bEnabled);

	/**
	 * CombBox ����֐����̌���I�������Ƃ��ɌĂяo�����
	 * ���݂� FunctionName �̒l���X�V����
	 */
	void OnSuggestSelected(TSharedPtr<FString> SelectedStringPtr, ESelectInfo::Type SelectInfo);


protected:
	/**
	 * ���� SourceClass �őI�����Ă���N���X���擾����
	 */
	const UClass* GetSelectedSourceClass() const;

	/**
	 * �w�肵���֐��� TemplateFunction �Ŏw�肳�ꂽ�����ƈ�v���邩�ǂ�����Ԃ�
	 */
	bool IsSignatureCompatible(const UFunction* Function) const;

	/**
	 * �ϐ��� Meta�w��q ���猻�݂� TemplateFunction ���X�V����
	 */
	bool UpdateTemplateFunction();

	/**
	 * Meta�w��q �̃L�[�ƒl���L�^����
	 */
	void CacheTemplateMetaKey();
};
