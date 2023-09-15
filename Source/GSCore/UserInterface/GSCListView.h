// Copyright (C) 2023 owoDra

#pragma once

#include "Components/ListView.h"

#include "GSCListView.generated.h"

class UUserWidget;
class UGSCData_ListVisual;
class UGSCData_SettingBase;


/**
 * 設定メニューなどに使用可能な設定をリスト表示するためのウィジェット
 * 
 * Tips:
 *  この ListView では ListEntry ウィジェットをすべて VisualData で決定している
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
	 * Data に応じたウィジェットクラスを返す
	 */
	virtual TSubclassOf<UUserWidget> GetEntryWidgetForData(const UGSCData_SettingBase* Data);
};
