// Copyright (C) 2023 owoDra

#pragma once

#include "Components/ListView.h"

#include "GSCListView.generated.h"

class UUserWidget;
class UGSCData_ListVisual;
class UGSCData_SettingBase;


/**
 * �ݒ胁�j���[�ȂǂɎg�p�\�Ȑݒ�����X�g�\�����邽�߂̃E�B�W�F�b�g
 * 
 * Tips:
 *  ���� ListView �ł� ListEntry �E�B�W�F�b�g�����ׂ� VisualData �Ō��肵�Ă���
 */
UCLASS()
class GSCORE_API UGSCListView : public UListView
{
public:
	GENERATED_BODY()
public:
	UGSCListView(const FObjectInitializer& ObjectInitializer);

#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(IWidgetCompilerLog& InCompileLog) const override;
#endif

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Config")
	TObjectPtr<UGSCData_ListVisual> VisualData;

protected:
	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;
	virtual bool OnIsSelectableOrNavigableInternal(UObject* SelectedItem) override;

	/**
	 * Data �ɉ������E�B�W�F�b�g�N���X��Ԃ�
	 */
	virtual TSubclassOf<UUserWidget> GetEntryWidgetForData(const UGSCData_SettingBase* Data);
};
