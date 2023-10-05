// Copyright (C) 2023 owoDra

#pragma once

#include "Data/GameSettingTypes.h"

#include "Data/EditCondition/GameSettingEditableState.h"

#include "GameSetting.generated.h"

class ULocalPlayer;
class UGameSettingRegistry;
class UGameSettingEditCondition;


/**
 * Reason why the value of the setting was changed
 */
UENUM(BlueprintType)
enum class EGameSettingChangeReason : uint8
{
	Change,				// Changed by player or others.

	DependencyChanged,	// Changed by the dependency settings.

	ResetToDefault,		// Reset to default values.

	RestoreChanges,		// Restored the changed value to previous value.
};


/**
 * The base class for managing game setting.
 * 
 * Tips:
 *  This class implements the main information such as name, description, dependencies, and edit conditions of the setting.
 */
UCLASS(Abstract, BlueprintType, DefaultToInstanced, EditInlineNew, NotBlueprintable, Within = "GameSettingRegistry")
class GSCORE_API UGameSetting : public UObject
{
	GENERATED_BODY()
public:
	UGameSetting() {}

public:
	DECLARE_EVENT_TwoParams(UGameSetting, FSettingChangedDelegate, UGameSetting* /*InSetting*/, EGameSettingChangeReason /*InChangeReason*/);
	FSettingChangedDelegate OnSettingChangedEvent;

	DECLARE_EVENT_OneParam(UGameSetting, FSettingAppliedDelegate, UGameSetting* /*InSetting*/);
	FSettingAppliedDelegate OnSettingAppliedEvent;

	DECLARE_EVENT_OneParam(UGameSetting, FSettingEditConditionChangedDelegate, UGameSetting* /*InSetting*/);
	FSettingEditConditionChangedDelegate OnSettingEditConditionChangedEvent;

protected:
	//
	// LocalPlayer that manages the GameSettingRegistry that owns this GameSetting.
	//
	UPROPERTY(Transient)
	TObjectPtr<ULocalPlayer> LocalPlayer{ nullptr };

	UPROPERTY(Transient)
	TObjectPtr<UGameSetting> ParentSetting{ nullptr };

	UPROPERTY(Transient)
	TObjectPtr<UGameSettingRegistry> OwningRegistry{ nullptr };

protected:
	//
	// Unique non-localized name to identify the setting
	//
	UPROPERTY(EditDefaultsOnly, Category = "GameSetting")
	FName DevName{ NAME_None };

	//
	// Localized name of the setting to be displayed in the UI.
	//
	UPROPERTY(EditDefaultsOnly, Category = "GameSetting")
	FText DisplayName;

	//
	// Localized description of the setting to be displayed in the UI.
	//
	UPROPERTY(EditDefaultsOnly, Category = "GameSetting")
	FText Description;

	//
	// Localized warning text of the setting to be displayed in the UI.
	// 
	// Tips:
	//  Write notes about the settings in this section.
	//
	UPROPERTY(AdvancedDisplay, EditDefaultsOnly, Category = "GameSetting")
	FText WarningText;

	//
	// Any edit conditions for this setting.
	//
	UPROPERTY(AdvancedDisplay, EditDefaultsOnly, Category = "GameSetting")
	TArray<TObjectPtr<UGameSettingEditCondition>> EditConditions;

private:
	//
	// Whether ready to access this setting.
	// 
	// Tips:
	//  Most settings are immediately ready, but some may require startup time before it's safe to call their functions.
	//
	bool bReady{ false };

	//
	// Flag to prevent re-entry problems when announcing that a setting has been changed.
	//
	bool bOnSettingChangedEventGuard{ false };

	//
	// Flag to prevent re-entrancy problems when announcing a setting has changed edit conditions.
	//
	bool bOnEditConditionsChangedEventGuard{ false };

	//
	// Cache the editable state of this setting when it changes rather than reprocessing it any time it's needed.
	//
	FGameSettingEditableState EditableStateCache;

	//
	// Whether searchable plain text should be refreshed.
	//
	mutable bool bShouldRefreshSearchablePlainText{ true };

	//
	// An auto-generated string that can be used to search for settings.
	//
	mutable FString SearchablePlainText;


public:
	/**
	 * Returns unique non-localized name to identify this setting
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameSettings")
	FName GetDevName() const { return DevName; }

	/**
	 * Returns localized name of this setting to be displayed in the UI.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameSettings")
	FText GetDisplayName() const { return DisplayName; }

	/**
	 * Returns localized description of this setting to be displayed in the UI.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameSettings")
	FText GetDescription() const { return Description; }

	/**
	 * Returns localized warning text of this setting to be displayed in the UI.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameSettings")
	FText GetWarningText() const { return WarningText; }


public:
	/** 
	 * Returns the searchable plain text for this setting. 
	 */
	const FString& GetSearchablePlainText() const;

	/**
	 * Invalidate the current searchable plane text and request an refresh.
	 */
	void InvalidateSearchablePlaneText() const { bShouldRefreshSearchablePlainText = true; }

protected:
	/**
	 * Refresh the searchable plain text.
	 */
	void RefreshSearchablePlainText() const;


public:
	/**
	 * Returns the LocalPlayer that owns this setting.
	 */
	ULocalPlayer* GetOwningLocalPlayer() const { return LocalPlayer; }

	/**
	 * Returns cached editable state of this setting.
	 */
	const FGameSettingEditableState& GetEditableState() const { return EditableStateCache; }

	/**
	 * Whether ready to access this setting.
	 * 
	 * Tips:
	 *  Most settings are immediately ready, but some may require startup time before it's safe to call their functions.
	 */
	bool IsReady() const { return bReady; }

	/**
	 * Any setting can have children, this is so we can allow for the possibility of "collections" or "actions" that
	 * are not directly visible to the user, but are set by some means and need to have initial and restored values.
	 * In that case, you would likely have internal settings inside an action subclass that is set on another screen,
	 * but never directly listed on the settings panel.
	 */
	virtual TArray<UGameSetting*> GetChildSettings() { return TArray<UGameSetting*>(); }

	/**
	 * Gets the current world of the local player that owns these settings. 
	 */
	virtual UWorld* GetWorld() const override;


public:
	/** 
	 * Initializes the setting, giving it the owning local player.
	 * 
	 * Tips:
	 *  Containers automatically initialize settings added to them. 
	 */
	void Initialize(ULocalPlayer* InLocalPlayer);

	void SetRegistry(UGameSettingRegistry* InOwningRegistry) { OwningRegistry = InOwningRegistry; }

	void SetSettingParent(UGameSetting* InSettingParent);

	void AddEditDependency(UGameSetting* DependencySetting);

protected:
	/**
	 * Call when this setting is initialized.
	 */
	virtual void OnInitialized();

	/**
	 * Perform the startup process of this setting.
	 *
	 * Tips:
	 *  Override this function with a setting that has a delay until becoming accessible.
	 * 
	 * Note:
	 *  When the startup process is complete, call StartupComplete() in the function to announce completion.
	 */
	virtual void Startup();
	void StartupComplete();


public:
	/**
	 * Refresh the editable state of this setting and notify that the state has changed.
	 */
	void RefreshEditableState(bool bNotifyEditConditionsChanged = true);

protected:
	/**
	 * Determine the current editable state
	 */
	FGameSettingEditableState ComputeEditableState() const;

	/**
	 * Called when a new editable state is known
	 */
	virtual void OnGatherEditState(FGameSettingEditableState& InOutEditState) const;

	/** 
	 * Notify that the settings edit conditions changed.  
	 * 
	 * Tips:
	 *  This may mean it's now invisible, or disabled, or possibly that the options have changed in some meaningful way. 
	 */
	void NotifyEditConditionsChanged();

	/**
	 * Called when editable state has changed
	 */
	virtual void OnEditConditionsChanged();


public:
	/**
	 * Explicitly apply settings.
	 * 
	 * Note:
	 *  This function is used for setting that does not actually apply the change immediately after the value is changed.
	 *  e.g., resolution setting.
	 */
	void Apply();

protected:
	virtual void OnApply();


protected:	
	/**
	 * Callback notifying that the value of a dependent setting has changed
	 */
	void HandleEditDependencyChanged(UGameSetting* DependencySetting, EGameSettingChangeReason Reason);
	void HandleEditDependencyChanged(UGameSetting* DependencySetting);

	/**
	 * Called when the value of a setting in a dependency is changed.
	 */
	virtual void OnDependencyChanged();


protected:
	/** 
	 * Notify that this setting changed 
	 */
	void NotifySettingChanged(EGameSettingChangeReason Reason);

	/**
	 * Called when this setting has changed.
	 */
	virtual void OnSettingChanged(EGameSettingChangeReason Reason);

};
