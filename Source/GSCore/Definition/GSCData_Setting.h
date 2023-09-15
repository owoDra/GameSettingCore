// Copyright (C) 2023 owoDra

#pragma once

#include "Engine/DataAsset.h"

#include "Definition/EditCondition/GSCEditableState.h"

#include "GSCData_Setting.generated.h"

class UGSCFormatBase;
class UGSCEditCondition;


/**
 * ユーザーがアクセス可能な設定項目の定義を行うデータアセット
 * 
 * Tips:
 *  このデータアセットはフォーマットデータを選択することで、それぞれのフォーマットに応じたデータの管理が可能になります。
 *  このデータアセット内で定義されるのは設定メニューなどでどのタイプのウィジェットか決めるためのデータのみで、細かな設定項目のデータは
 *  フォーマットデータ内で定義する。
 */
UCLASS(BlueprintType, meta = (DisplayName = "SettingData"))
class GSCORE_API UGSCData_Setting : public UDataAsset
{
public:
	GENERATED_BODY()
public:
	UGSCData_Setting(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) override;
#endif // WITH_EDITOR

protected:
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
	// 設定メニューでこの設定に対してだけ特殊なウィジェットを適用させたい時参照する名前
	// 
	// Tips:
	//  基本的には None のままで問題ない。
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GameSettings")
	FName SpecificName{ NAME_None };

	//
	// この SettingData の役割と定義するフォーマットデータ
	// 
	// 注意:
	//  この項目は必ず設定しなければいけません。
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced, Category = "GameSettings")
	TObjectPtr<UGSCFormatBase> Format{ nullptr };

	//
	// この SettingData で定義された設定項目の編集可能状態を決定するデータ
	// 
	// Tips:
	//  この項目を設定しない場合、この設定項目は常に編集可能な状態になります
	//
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced, Category = "GameSettings")
	TObjectPtr<UGSCEditCondition> EditCondition{ nullptr };

public:
	//
	// 他の SettingData などによって SettingData の更新リクエストがあったときに呼び出されるデリゲート
	// 
	// Tips:
	//  更新リクエスト時に UI を更新するといった処理を実装する際に使用可能
	//
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGSCRefreshSettingRequestDelegate);
	UPROPERTY(BlueprintAssignable, Transient)
	FGSCRefreshSettingRequestDelegate OnRequestRefreshSetting;

public:
	/**
	 * この SettingData で定義された設定項目の状態を更新リクエストする
	 *
	 * Tips:
	 *  この関数は基本的に 他の設定項目 の値を変更した後に関連する この設定項目 を更新しなければいけない時や
	 *  その他のシステムによって この設定項目 の状態を更新しなければいけないときに実行される。
	 */
	UFUNCTION(BlueprintCallable, Category = "GameSetting")
	void RequestRefreshSetting();


public:
	/**
	 * この SettingData がすでに廃止されたものかどうかを返す
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameSetting")
	bool IsDeprecated() const { return bDeprecated; }

	/**
	 * SpecificName を返す
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameSetting")
	virtual const FName& GetSpecificName() const { return SpecificName; }

	/**
	 * この SettingData で定義された設定項目の編集可能状態を返す
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameSetting")
	FGSCEditableState GetEditableState() const;

	/**
	 * この SettingData のフォーマットを返す
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameSetting", meta = (DeterminesOutputType = "FormatClass", AllowAbstract))
	const UGSCFormatBase* GetFormatByClass(TSubclassOf<UGSCFormatBase> FormatClass) const { return Format; }

	template<class FormatClass = UGSCFormatBase>
	const FormatClass* GetFormat() const
	{
		return Cast<FormatClass>(Format);
	}

	/**
	 * この SettingData のフォーマットのクラスを返す
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameSetting")
	TSubclassOf<UGSCFormatBase> GetFormatClass() const;

};
