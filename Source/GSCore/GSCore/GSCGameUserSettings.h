// Copyright (C) 2023 owoDra

#pragma once

#include "GameFramework/GameUserSettings.h"

#include "GSCSubsystem.h"

#include "Subsystems/SubsystemCollection.h"

#include "GSCGameUserSettings.generated.h"

class UGSCSubsystem;


/**
 * Game User Settings with extensible subsystems that can be added
 */
UCLASS(BlueprintType)
class GSCORE_API UGSCGameUserSettings : public UGameUserSettings
{
public:
	GENERATED_BODY()
public:
	UGSCGameUserSettings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

private:
	FSubsystemCollection<UGSCSubsystem> SubsystemCollection;

protected:
	/**
	 * Executed when the instance is created and initializes subsystems
	 */
	void InitializeSubsystems();

	/**
	 * Executed when the instance is destroyed and deinitialize subsystems
	 */
	void DeinitializeSubsystems();

public:
	static UGSCGameUserSettings* GetGSCGameUserSettings();

	/**
	 * Get a Subsystem of specified type
	 */
	UGSCSubsystem* GetSubsystemBase(TSubclassOf<UGSCSubsystem> SubsystemClass) const
	{
		return SubsystemCollection.GetSubsystem<UGSCSubsystem>(SubsystemClass);
	}

	/**
	 * Get a Subsystem of specified type
	 */
	template <typename TSubsystemClass>
	TSubsystemClass* GetSubsystem() const
	{
		return SubsystemCollection.GetSubsystem<TSubsystemClass>(TSubsystemClass::StaticClass());
	}

	/**
	 * Get a Subsystem of specified type from the provided GameUserSettings
	 * returns nullptr if the Subsystem cannot be found or the GameUserSettings is null
	 */
	template <typename TSubsystemClass>
	static TSubsystemClass* GetSubsystem(const UGSCGameUserSettings* GameUserSettings)
	{
		return GameUserSettings ? GameUserSettings->GetSubsystem<TSubsystemClass>() : nullptr;
	}

	/**
	 * Get all Subsystem of specified type, this is only necessary for interfaces that can have multiple implementations instanced at a time.
	 *
	 * Note:
	 *	Do not hold onto this Array reference unless you are sure the lifetime is less than that of UGameUserSettings
	 */
	template <typename TSubsystemClass>
	const TArray<TSubsystemClass*>& GetSubsystemArray() const
	{
		return SubsystemCollection.GetSubsystemArray<TSubsystemClass>(TSubsystemClass::StaticClass());
	}

	/**
	 * Get a Subsystem of specified type from the provided GameUserSettings
	 * returns nullptr if the Subsystem cannot be found or the GameUserSettings is null
	 */
	template <typename TSubsystemClass>
	static TSubsystemClass* GetSettingSubsystem()
	{
		auto* GUS{ UGSCGameUserSettings::GetGSCGameUserSettings() };

		return GUS ? GUS->GetSubsystem<TSubsystemClass>() : nullptr;
	}

public:
	virtual void PostInitProperties() override;
	virtual void BeginDestroy() override;


public:
	virtual void SetToDefaults() override;
	virtual void ValidateSettings() override;
	virtual void LoadSettings(bool bForceReload = false) override;
	virtual void SaveSettings() override;
	virtual void ApplySettings(bool bCheckForCommandLineOverrides) override;
	virtual void ResetToCurrentSettings() override;

	/**
	 * Dirty or not in all subsystems in addition to the result of UGameUserSettings::IsDirty()
	 */
	virtual bool IsDirty() const override;

	/**
	 * Same as UGameUserSettings::IsDirty()
	 * 
	 * Tips:
	 *	The result of IsDirty() in subsystems is not considered.
	 */
	virtual bool IsDirtyIgnoreSubsystems() const;


public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FGameSettingsAppliedDelegate, UGSCGameUserSettings* Settings);
	FGameSettingsAppliedDelegate OnGameSettingsApplied;

public:
	FDelegateHandle CallAndRegister_OnGameSettingsApplied(FGameSettingsAppliedDelegate::FDelegate Delegate);

};
