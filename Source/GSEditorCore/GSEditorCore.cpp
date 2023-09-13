// Copyright (C) 2023 owoDra

#include "GSEditorCore.h"

#include "PropertyCustomization/GSCSettingSourcePickerCustomization.h"
#include "PropertyCustomization/GSCPropertySourcePickerCustomization.h"

IMPLEMENT_MODULE(FGSEditorCoreModule, GSEditorCore)


void FGSEditorCoreModule::StartupModule()
{
	RegisterPropertyCustomizations();
}

void FGSEditorCoreModule::ShutdownModule()
{
	UnregisterPropertyCustomizations();
}


void FGSEditorCoreModule::RegisterPropertyCustomizations()
{
	if (!FModuleManager::Get().IsModuleLoaded(NAME_PropertyEditorModule))
	{
		return;
	}

	auto& PropertyModule{ FModuleManager::LoadModuleChecked<FPropertyEditorModule>(NAME_PropertyEditorModule) };

	FGSCSettingSourcePickerCustomization::RegisterToPropertyEditorModule(PropertyModule);
	FGSCPropertySourcePickerCustomization::RegisterToPropertyEditorModule(PropertyModule);

	PropertyModule.NotifyCustomizationModuleChanged();
}

void FGSEditorCoreModule::UnregisterPropertyCustomizations()
{
	if (!FModuleManager::Get().IsModuleLoaded(NAME_PropertyEditorModule))
	{
		return;
	}

	auto& PropertyModule{ FModuleManager::LoadModuleChecked<FPropertyEditorModule>(NAME_PropertyEditorModule) };

	FGSCSettingSourcePickerCustomization::UnregisterToPropertyEditorModule(PropertyModule);
	FGSCPropertySourcePickerCustomization::UnregisterToPropertyEditorModule(PropertyModule);
}
