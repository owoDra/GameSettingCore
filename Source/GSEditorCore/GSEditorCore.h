// Copyright (C) 2023 owoDra

#pragma once

#include "Modules/ModuleManager.h"

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

};
