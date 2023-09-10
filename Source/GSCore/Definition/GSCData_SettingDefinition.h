// Copyright (C) 2023 owoDra

#pragma once

#include "Definition/GSCData_SettingBase.h"

#include "GameplayTagContainer.h"

#include "GSCData_SettingDefinition.generated.h"

class UGSCFormatBase;


/**
 * 設定項目の定義に使用するデータアセット
 * 
 * 保有するデータ:
 *  - 設定の表示名
 *  - 設定の説明
 *  - 設定のタグ (特定の設定項目への関連付けなどに使用可能)
 *  - 設定のデータソースへのアクセスパス
 *  - 設定の型ごとの追加情報
 */
UCLASS(BlueprintType, Const)
class GSCORE_API UGSCData_SettingDefinition : public UGSCData_SettingBase
{
public:
	GENERATED_BODY()
public:
	UGSCData_SettingDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	//
	// 設定がもう使用されていないか
	// 
	// Tips:
	//  廃止予定、または一時的に使用不可にする場合は true にしてください。
	//  また、設定を完全に削除する場合は定義から項目自体を削除してください。
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Misc")
	bool bDeprecated{ false };

	//
	// この設定を識別や特定の設定との関連を表すのに使用可能な GameplayTag
	// 
	// Tips:
	//  必ずしもつける必要はない。
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Setting|Tags")
	FGameplayTag SettingTag;

	//
	// この設定が更新される際に、指定されたこれらの GameplayTag を持つ設定項目に対して
	// UI 更新リクエストを行う
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Setting|Tags")
	TArray<FGameplayTag> UpdateRequestTags;

	//
	// この設定のデータ型をごとの定義情報の設定
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced, Category = "Setting")
	TObjectPtr<const UGSCFormatBase> Format;
};
