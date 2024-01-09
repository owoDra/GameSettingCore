// Copyright (C) 2024 owoDra

#include "GSEditorCore.h"

IMPLEMENT_MODULE(FGSEditorCoreModule, GSEditorCore)


#define LOCTEXT_NAMESPACE "GSEditorCoreModule"

void FGSEditorCoreModule::StartupModule()
{
	RegisterPropertyCustomizations();
	RegisterAssetTypeActions();
}

void FGSEditorCoreModule::ShutdownModule()
{
	UnregisterPropertyCustomizations();
	UnregisterAssetTypeActions();
}


void FGSEditorCoreModule::RegisterPropertyCustomizations()
{
	if (!FModuleManager::Get().IsModuleLoaded(NAME_PropertyEditorModule))
	{
		return;
	}

	auto& PropertyModule{ FModuleManager::LoadModuleChecked<FPropertyEditorModule>(NAME_PropertyEditorModule) };

	PropertyModule.NotifyCustomizationModuleChanged();
}

void FGSEditorCoreModule::UnregisterPropertyCustomizations()
{
	if (!FModuleManager::Get().IsModuleLoaded(NAME_PropertyEditorModule))
	{
		return;
	}

	auto& PropertyModule{ FModuleManager::LoadModuleChecked<FPropertyEditorModule>(NAME_PropertyEditorModule) };
}


void FGSEditorCoreModule::RegisterAssetTypeActionCategory()
{
	static const FName CategoryKey{ TEXT("GameSettingsCore") };
	static const FText CategoryDisplayName{ LOCTEXT("GameSettingsCoreCategory", "Game Settings Core") };
	SettingsCategory = IAssetTools::Get().RegisterAdvancedAssetCategory(CategoryKey, CategoryDisplayName);
}

void FGSEditorCoreModule::RegisterAssetTypeActions()
{
	RegisterAssetTypeActionCategory();
}

void FGSEditorCoreModule::UnregisterAssetTypeActions()
{
	UnregisterAssets(RegisteredAssetTypeActions);
}

void FGSEditorCoreModule::UnregisterAssets(TArray<TSharedPtr<FAssetTypeActions_Base>>& RegisteredAssets)
{
	const auto* AssetToolsPtr{ FModuleManager::GetModulePtr<FAssetToolsModule>(NAME_AssetToolsModule) };
	if (!AssetToolsPtr)
	{
		return;
	}

	auto& AssetTools{ AssetToolsPtr->Get() };
	for (auto& AssetTypeActionIt : RegisteredAssets)
	{
		if (AssetTypeActionIt.IsValid())
		{
			AssetTools.UnregisterAssetTypeActions(AssetTypeActionIt.ToSharedRef());
		}
	}
}

#undef LOCTEXT_NAMESPACE
