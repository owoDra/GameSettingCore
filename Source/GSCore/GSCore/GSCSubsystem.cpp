// Copyright (C) 2023 owoDra

#include "GSCSubsystem.h"

#include "GSCoreLogs.h"

#include "Misc/ConfigContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSCSubsystem)


FString UGSCSubsystem::GetCustomConfigIniName() const
{
    return FString();
}

void UGSCSubsystem::LoadConfigIni(bool bForceReload)
{
    auto Context{ FConfigContext::ReadIntoGConfig() };
    Context.bForceReload = bForceReload;
    Context.Load(*GetCustomConfigIniName(), GSCSubsystemConfigIni);
}

void UGSCSubsystem::LoadSettings(bool bForceReload)
{
	QUICK_SCOPE_CYCLE_COUNTER(GSCSubsystem_LoadSettings);

    const auto bHasCustomConfigIni{ !GetCustomConfigIniName().IsEmpty()};

    if (bForceReload && bHasCustomConfigIni)
    {
        LoadConfigIni(bForceReload);
    }
    
    LoadConfig(GetClass(), bHasCustomConfigIni ? *GSCSubsystemConfigIni : *GGameUserSettingsIni);
}

void UGSCSubsystem::SaveSettings()
{
    QUICK_SCOPE_CYCLE_COUNTER(GSCSubsystem_SaveSettings);

    const auto bHasCustomConfigIni{ !GetCustomConfigIniName().IsEmpty() };

    if (bHasCustomConfigIni)
    {
        const auto bCustomConfigIniNameExist{ !GSCSubsystemConfigIni.IsEmpty() };

        if (!bCustomConfigIniNameExist)
        {
            LoadConfigIni();
        }
    }

    SaveConfig(CPF_Config, bHasCustomConfigIni ? *GSCSubsystemConfigIni : *GGameUserSettingsIni);
}


void UGSCSubsystem::ApplySettings()
{
    ClearDirty();
}


void UGSCSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    UE_LOG(LogGSC, Log, TEXT("Initialized Settings Subsystem(%s)"), *GetNameSafe(this));
}
