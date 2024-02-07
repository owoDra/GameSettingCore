// Copyright (C) 2024 owoDra

#include "GSCGameUserSettings.h"

#include "GSCoreLogs.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSCGameUserSettings)


UGSCGameUserSettings::UGSCGameUserSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetToDefaults();
}


void UGSCGameUserSettings::InitializeSubsystems()
{
	UE_LOG(LogGameCore_Settings, Log, TEXT("=== Initializing GameUserSettings(%s) ==="), *GetNameSafe(this));

	SubsystemCollection.Initialize(this);
}

void UGSCGameUserSettings::DeinitializeSubsystems()
{
	SubsystemCollection.Deinitialize();
}


UGSCGameUserSettings* UGSCGameUserSettings::GetGSCGameUserSettings()
{
	return Cast<UGSCGameUserSettings>(UGameUserSettings::GetGameUserSettings());
}


void UGSCGameUserSettings::PostInitProperties()
{
	InitializeSubsystems();

	Super::PostInitProperties();
}

void UGSCGameUserSettings::BeginDestroy()
{
	DeinitializeSubsystems();

	Super::BeginDestroy();
}


void UGSCGameUserSettings::SetToDefaults()
{
	Super::SetToDefaults();

	for (const auto& Each : GetSubsystemArray<UGSCSubsystem>())
	{
		Each->SetToDefaults();
	}
}

void UGSCGameUserSettings::LoadSettings(bool bForceReload)
{
	Super::LoadSettings(bForceReload);

	for (const auto& Each : GetSubsystemArray<UGSCSubsystem>())
	{
		Each->LoadSettings(bForceReload);
	}
}

void UGSCGameUserSettings::ValidateSettings()
{
	Super::ValidateSettings();

	for (const auto& Each : GetSubsystemArray<UGSCSubsystem>())
	{
		Each->ValidateSettings();
	}
}

void UGSCGameUserSettings::SaveSettings()
{
	Super::SaveSettings();

	for (const auto& Each : GetSubsystemArray<UGSCSubsystem>())
	{
		Each->SaveSettings();
	}
}

void UGSCGameUserSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
	for (const auto& Each : GetSubsystemArray<UGSCSubsystem>())
	{
		Each->ApplySettings();
	}

	Super::ApplySettings(bCheckForCommandLineOverrides);

	OnGameSettingsApplied.Broadcast(this);
}

void UGSCGameUserSettings::ResetToCurrentSettings()
{
	Super::ResetToCurrentSettings();

	for (const auto& Each : GetSubsystemArray<UGSCSubsystem>())
	{
		Each->ResetToCurrentSettings();
	}
}

bool UGSCGameUserSettings::IsDirty() const
{
	QUICK_SCOPE_CYCLE_COUNTER(GSCGameUserSettings_IsDirty);

	// Skip process if already dirty

	if (Super::IsDirty())
	{
		UE_LOG(LogGameCore_Settings, Log, TEXT("UGSCGameUserSettings::IsDirty(): GSCGameUserSettings is dirty"));
		return true;
	}

	// Verify all subsystems

	for (const auto& Each : GetSubsystemArray<UGSCSubsystem>())
	{
		if (Each->IsDirty())
		{
			UE_LOG(LogGameCore_Settings, Log, TEXT("UGSCGameUserSettings::IsDirty(): %s is dirty"), *GetNameSafe(Each));
			return true;
		}
	}

	return false;
}

bool UGSCGameUserSettings::IsDirtyIgnoreSubsystems() const
{
	return Super::IsDirty();
}


FDelegateHandle UGSCGameUserSettings::CallAndRegister_OnGameSettingsApplied(FGameSettingsAppliedDelegate::FDelegate Delegate)
{
	Delegate.Execute(this);

	return OnGameSettingsApplied.Add(Delegate);
}
