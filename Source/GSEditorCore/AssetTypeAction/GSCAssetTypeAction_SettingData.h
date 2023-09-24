// Copyright (C) 2023 owoDra

#pragma once

#include "Factories/Factory.h"
#include "AssetTypeActions_Base.h"

#include "GSCAssetTypeAction_SettingData.generated.h"

class UGSCFormatBase;


/**
 * アセット作成のコンテキストメニューでSettingData の作成を行うクラス
 */
UCLASS()
class UGSCSettingDataFactory : public UFactory
{
public:
	GENERATED_BODY()
public:
	UGSCSettingDataFactory();

protected:
	UPROPERTY(EditAnywhere, Category = "SettingData")
	TSubclassOf<UGSCFormatBase> FormatClass{ nullptr };

public:
	virtual FText GetDisplayName() const override;
	virtual bool ShouldShowInNewMenu() const override { return true; }
	virtual bool DoesSupportClass(UClass* Class) override;
	virtual bool ConfigureProperties() override;

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

};


/**
 * アセット作成のコンテキストメニューに SettingData の作成項目を追加するクラス
 */
class FGSCAssetTypeActions_SettingData : public FAssetTypeActions_Base
{
public:
	virtual ~FGSCAssetTypeActions_SettingData() override = default;

public:
	virtual FText GetName() const override;
	virtual FText GetAssetDescription(const FAssetData& AssetData) const override;
	virtual FColor GetTypeColor() const override;
	virtual uint32 GetCategories() override;
	virtual UClass* GetSupportedClass() const override;

};
