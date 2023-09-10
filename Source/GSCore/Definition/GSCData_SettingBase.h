// Copyright (C) 2023 owoDra

#pragma once

#include "Engine/DataAsset.h"

#include "GSCData_SettingBase.generated.h"


/**
 * 設定項目の定義を集めたデータアセット
 */
UCLASS(Abstract, BlueprintType, Const)
class GSCORE_API UGSCData_SettingBase : public UDataAsset
{
public:
	GENERATED_BODY()
public:
	UGSCData_SettingBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	//
	// 設定のメニューなどに表示される名前
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Setting")
	FText Name;

	//
	// 設定のメニューなどに表示される説明
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Setting")
	FText Discription;

};
