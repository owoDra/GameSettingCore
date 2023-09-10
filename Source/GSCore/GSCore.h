// Copyright (C) 2023 owoDra

#pragma once

#include "Modules/ModuleManager.h"

/**
 *  GameSettingsCore プラグインの主要機能のモジュール
 */
class FGSCoreModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};
