// Copyright (C) 2023 owoDra

#pragma once

#include "Definition/GSCData_SettingBase.h"

#include "GSCData_SettingCollection.generated.h"

class UGSCData_SettingDefinition;


/**
 * 設定項目の定義を集めたデータアセット
 */
UCLASS(BlueprintType, Const)
class GSCORE_API UGSCData_SettingCollection : public UGSCData_SettingBase
{
public:
	GENERATED_BODY()
public:
	UGSCData_SettingCollection(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	//
	// 定義データのリスト
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Collection")
	TArray<TObjectPtr<UGSCData_SettingBase>> Collection;

};
