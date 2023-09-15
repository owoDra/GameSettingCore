// Copyright (C) 2023 owoDra

#pragma once

#include "Engine/DataAsset.h"

#include "GameplayTagContainer.h"

#include "GSCData_ListVisual.generated.h"

class UUserWidget;
class UGSCFormatBase;
class UGSCData_SettingBase;


/**
 * GSCListView �ɕ\���\�ȃE�B�W�F�b�g�N���X��I�����邽�߂̃f�[�^
 */
USTRUCT(BlueprintType)
struct FGSCPicker_ListEntry
{
public:
	GENERATED_BODY()
public:
	FGSCPicker_ListEntry() {}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (MustImplement = "GSCListEntryWidgetInterface"))
	TSubclassOf<UUserWidget> WidgetClass{ nullptr };

};


/**
 * GSCListView �Ŏg�p����ݒ�̎�ނ��ƂɎg�p����E�B�W�F�b�g�����߂�f�[�^�A�Z�b�g
 */
UCLASS(BlueprintType, Const)
class GSCORE_API UGSCData_ListVisual : public UDataAsset
{
public:
	GENERATED_BODY()

protected:
	//
	// ���X�g���̃Z�N�V�������Ƃ̃w�b�_�[�Ɏg�p����E�B�W�F�b�g�N���X
	// 
	// Tips:
	//  �f�t�H���g�ł� GSCListView �ɕ\�������悤�Ƃ��� SettingCollection ���̃T�u SettingCollection ��
	//  ���O���w�b�_�[�Ƃ��Ďg�p�����B
	//  ���̃w�b�_�[������̃��[���� GSCListView ����h���N���X���쐬���邱�ƂŕύX�\�B
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "ListView")
	FGSCPicker_ListEntry EntryWidgetForHeader;

	//
	// �ݒ��`�f�[�^�̃t�H�[�}�b�g���ƂɎg�p����E�B�W�F�b�g�N���X�̃}�b�s���O���X�g
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "ListView")
	TMap<TSubclassOf<UGSCFormatBase>, FGSCPicker_ListEntry> EntryWidgetsForFormat;

	//
	// �ݒ��`�f�[�^�� SettingTag ���ƂɎg�p����E�B�W�F�b�g�N���X�̃}�b�s���O���X�g
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "ListView")
	TMap<FGameplayTag, FGSCPicker_ListEntry> EntryWidgetsForTag;

public:
	/**
	 * �w�肵�� Data �ɑΉ����� EntryWidget �̃N���X��Ԃ�
	 * �L���ȃN���X��������Ȃ������ꍇ�� nullptr ��Ԃ�
	 */
	TSubclassOf<UUserWidget> GetEntryWidgetForData(const UGSCData_SettingBase* InData);

};
