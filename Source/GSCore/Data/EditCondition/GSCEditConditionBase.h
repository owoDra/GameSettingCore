// Copyright (C) 2023 owoDra

#pragma once

#include "UObject/Object.h"

#include "GSCEditableState.h"

#include "GSCEditConditionBase.generated.h"


/**
 * ユーザーが変更可能なゲーム設定の変更可能条件を定義する
 */
UCLASS(Abstract, Blueprintable, DefaultToInstanced, EditInlineNew, Within = "GSCData_Setting")
class GSCORE_API UGSCEditConditionBase : public UObject
{
public:
	GENERATED_BODY()

	friend class UGSCData_Setting;

public:
	//
	// 他の SettingData などによって SettingData の更新リクエストがあったときに UI に対して編集可能状態の更新を知らせるデリゲート
	//
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGSCRefreshEditableStateRequestDelegate, FGSCEditableState, EditableState);
	UPROPERTY(BlueprintAssignable, Transient)
	FGSCRefreshEditableStateRequestDelegate OnRequestRefreshEditableState;

protected:
	/**
	 * 現在の設定の編集可能状態を確認する
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "EditCondition")
	FGSCEditableState GatherEditableState();
	virtual FGSCEditableState GatherEditableState_Implementation() { return FGSCEditableState::Editable; }

	/**
	 * この Format を所有する SettingData に対して更新リクエストが行われたときに実行する
	 */
	virtual void HandleRefreshSettingRequest();

};
