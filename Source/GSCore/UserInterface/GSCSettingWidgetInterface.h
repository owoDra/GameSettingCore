// Copyright (C) 2023 owoDra

#pragma once

#include "UObject/Interface.h"

#include "GSCSettingWidgetInterface.generated.h"

class UGSCData_Setting;


/** 
 * SettingData を登録して使用するウィジェットのためのインターフェイス
 */
UINTERFACE(BlueprintType, Blueprintable, meta = (DisplayName = "SettingWidgetInterface"))
class GSCORE_API UGSCSettingWidgetInterface : public UInterface
{
public:
	GENERATED_BODY()

};

class GSCORE_API IGSCSettingWidgetInterface
{
public:
	GENERATED_BODY()

public:
	/**
	 * ウィジェットの生成に使用したデータを登録する
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "GameSettings")
	void RegisterData(const UGSCData_Setting* InData);

};
