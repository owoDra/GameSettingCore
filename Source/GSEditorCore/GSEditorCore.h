// Copyright (C) 2024 owoDra

#pragma once

#include "Modules/ModuleManager.h"

#include "AssetTypeCategories.h"
#include "AssetTypeActions_Base.h"
#include "IAssetTools.h"


/**
 *  GameSettingsCore プラグインのエディター機能のモジュール
 */
class FGSEditorCoreModule : public IModuleInterface
{
public:
	//
	// PropertyEditorModule の ロード/アンロード を行うために使用するモジュール名
	//
	inline static const FName NAME_PropertyEditorModule{ TEXT("PropertyEditor") };

	//
	// AssetToolsModule の ロード/アンロード を行うために使用するモジュール名
	//
	inline static const FName NAME_AssetToolsModule{ TEXT("AssetTools") };

	//
	// アセット作成のコンテキストメニュー内のカテゴリー
	//
	inline static EAssetTypeCategories::Type SettingsCategory = EAssetTypeCategories::None;

public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

protected:
	/**
	 *  PropertyCustomizations をモジュールに登録する
	 */
	void RegisterPropertyCustomizations();

	/**
	 *  PropertyCustomizations をモジュールから登録解除する
	 */
	void UnregisterPropertyCustomizations();


protected:
	//
	// 登録した AssetTypeActions
	//
	TArray<TSharedPtr<FAssetTypeActions_Base>> RegisteredAssetTypeActions;

protected:
	/**
	 *  アセット作成のコンテキストメニュー内のカテゴリーを追加する
	 */
	void RegisterAssetTypeActionCategory();

	/**
	 *  AssetTypeActions をモジュールに登録する
	 */
	void RegisterAssetTypeActions();

	/**
	 *  AssetTypeActions をモジュールから登録解除する
	 */
	void UnregisterAssetTypeActions();

	template <typename T>
	void RegisterAsset(TArray<TSharedPtr<FAssetTypeActions_Base>>& InOutRegisteredAssets);

	void UnregisterAssets(TArray<TSharedPtr<FAssetTypeActions_Base>>& RegisteredAssets);
};

template<typename T>
inline void FGSEditorCoreModule::RegisterAsset(TArray<TSharedPtr<FAssetTypeActions_Base>>& InOutRegisteredAssets)
{
	TSharedPtr<T> SettingsDataTableAction{ MakeShared<T>() };
	IAssetTools::Get().RegisterAssetTypeActions(SettingsDataTableAction.ToSharedRef());
	InOutRegisteredAssets.Emplace(MoveTemp(SettingsDataTableAction));
}
