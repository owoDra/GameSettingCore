// Copyright (C) 2023 owoDra

#include "GSCSubsystem.h"

#include "Development/GSCDeveloperSettings.h"
#include "Development/GSCLogChannels.h"
#include "Subsystem/GSCSettingSourceInterface.h"
#include "Definition/GSCData_SettingDefinition.h"
#include "Definition/Picker/GSCPicker_PropertySource.h"

#include "PropertyPathHelpers.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSCSubsystem)


UObject* UGSCSubsystem::LoadOrGetSettingSourceObject(FName SettingSourceName)
{
	// キャッシュ済みの SettingSource か確認

	if (const auto* SourceDelegate{ CachedSettingSources.Find(SettingSourceName) })
	{
		return SourceDelegate->Execute();
	}

	// キャッシュされていなかった場合

	else
	{
		const auto* Setting{ GetDefault<UGSCDeveloperSettings>() };

		const auto* SettingSource{ Setting->SettingSources.Find(SettingSourceName) };
		if (!ensureMsgf(SettingSource, TEXT("Invalid SettingSourceName")))
		{
			return nullptr;
		}

		const auto* SourceClass{ SettingSource->SourceClass.TryLoadClass<UObject>() };
		if (!ensureMsgf(SourceClass, TEXT("Invalid SourceClass")))
		{
			return nullptr;
		}

		FSettingSourceStaticContext NewSourceDelegate;
		NewSourceDelegate.BindUFunction(SourceClass->GetDefaultObject(), SettingSource->FunctionName);
		if (!ensureMsgf(NewSourceDelegate.IsBound(), TEXT("Failed to bind UFunction(%s::%s)"), *GetNameSafe(SourceClass->GetDefaultObject()), *SettingSource->FunctionName.ToString()))
		{
			return nullptr;
		}

		CachedSettingSources.Add(SettingSourceName, NewSourceDelegate);

		return NewSourceDelegate.Execute();
	}
}


#pragma region SettingSourceRequests

void UGSCSubsystem::RequestApplySettingsForAll()
{
	const auto* Setting{ GetDefault<UGSCDeveloperSettings>() };
	const auto SettingSources{ Setting->SettingSources };

	for (const auto& KVP : SettingSources)
	{
		auto* SourceObject{ LoadOrGetSettingSourceObject(KVP.Key) };
		if (!SourceObject)
		{
			// Tips: 現時点でまだ SourceObject の準備ができておらずにアクセスできない場合がある

			continue;
		}

		auto Interface{ Cast<IGSCSettingSourceInterface>(SourceObject) };
		if (!ensureMsgf(Interface, TEXT("%s does not implement IGSCSettingSourceInterface"), *GetNameSafe(SourceObject)))
		{
			continue;
		}

		Interface->RequestApplySettings();
	}
}

bool UGSCSubsystem::RequestApplySettings(FName SettingSourceName)
{
	auto* SourceObject{ LoadOrGetSettingSourceObject(SettingSourceName) };
	if (!ensureMsgf(SourceObject, TEXT("Request made when SettingSource(%s) was not ready"), *SettingSourceName.ToString()))
	{
		return false;
	}

	auto Interface{ Cast<IGSCSettingSourceInterface>(SourceObject) };
	if (!ensureMsgf(Interface, TEXT("%s does not implement IGSCSettingSourceInterface"), *GetNameSafe(SourceObject)))
	{
		return false;
	}

	return Interface->RequestResetSettings();
}


void UGSCSubsystem::RequestResetSettingsForAll()
{
	const auto* Setting{ GetDefault<UGSCDeveloperSettings>() };
	const auto SettingSources{ Setting->SettingSources };

	for (const auto& KVP : SettingSources)
	{
		auto* SourceObject{ LoadOrGetSettingSourceObject(KVP.Key) };
		if (!SourceObject)
		{
			// Tips: 現時点でまだ SourceObject の準備ができておらずにアクセスできない場合がある

			continue;
		}

		auto Interface{ Cast<IGSCSettingSourceInterface>(SourceObject) };
		if (!ensureMsgf(Interface, TEXT("%s does not implement IGSCSettingSourceInterface"), *GetNameSafe(SourceObject)))
		{
			continue;
		}

		Interface->RequestResetSettings();
	}
}

bool UGSCSubsystem::RequestResetSettings(FName SettingSourceName)
{
	auto* SourceObject{ LoadOrGetSettingSourceObject(SettingSourceName) };
	if (!ensureMsgf(SourceObject, TEXT("Request made when SettingSource(%s) was not ready"), *SettingSourceName.ToString()))
	{
		return false;
	}

	auto Interface{ Cast<IGSCSettingSourceInterface>(SourceObject) };
	if (!ensureMsgf(Interface, TEXT("%s does not implement IGSCSettingSourceInterface"), *GetNameSafe(SourceObject)))
	{
		return false;
	}

	return Interface->RequestResetSettings();
}


void UGSCSubsystem::RequestReloadSettingsForAll()
{
	const auto* Setting{ GetDefault<UGSCDeveloperSettings>() };
	const auto SettingSources{ Setting->SettingSources };

	for (const auto& KVP : SettingSources)
	{
		auto* SourceObject{ LoadOrGetSettingSourceObject(KVP.Key) };
		if (!SourceObject)
		{
			// Tips: 現時点でまだ SourceObject の準備ができておらずにアクセスできない場合がある

			continue;
		}

		auto Interface{ Cast<IGSCSettingSourceInterface>(SourceObject) };
		if (!ensureMsgf(Interface, TEXT("%s does not implement IGSCSettingSourceInterface"), *GetNameSafe(SourceObject)))
		{
			continue;
		}

		Interface->RequestReloadSettings();
	}
}

bool UGSCSubsystem::RequestReloadSettings(FName SettingSourceName)
{
	auto* SourceObject{ LoadOrGetSettingSourceObject(SettingSourceName) };
	if (!ensureMsgf(SourceObject, TEXT("Request made when SettingSource(%s) was not ready"), *SettingSourceName.ToString()))
	{
		return false;
	}

	auto Interface{ Cast<IGSCSettingSourceInterface>(SourceObject) };
	if (!ensureMsgf(Interface, TEXT("%s does not implement IGSCSettingSourceInterface"), *GetNameSafe(SourceObject)))
	{
		return false;
	}

	return Interface->RequestReloadSettings();
}

#pragma endregion


#pragma region SettingProperties

bool UGSCSubsystem::GetSettingValueAsString(const UGSCData_SettingDefinition* Definition, FString& OutValue)
{
	OutValue = FString();

	if (!Definition)
	{
		return false;
	}

	const auto GetterSource{ Definition->GetGetterSource() };
	if (!GetterSource.IsValid())
	{
		UE_LOG(LogGSC, Warning, TEXT("Invalid GetterSource defined in [%s]"), *GetNameSafe(Definition));
		return false;
	}

	auto* SourceObject{ LoadOrGetSettingSourceObject(GetterSource.SettingSourceName) };
	if (!SourceObject)
	{
		UE_LOG(LogGSC, Warning, TEXT("Could not find SourceObject from SettingSourceName(%s) of GetterSource defined in [%s]"),
																	*GetterSource.SettingSourceName.ToString(), *GetNameSafe(Definition));
		return false;
	}

	FCachedPropertyPath DynamicPath{ GetterSource.FunctionName.ToString() };

	if (!PropertyPathHelpers::GetPropertyValueAsString(SourceObject, DynamicPath, OutValue))
	{
		UE_LOG(LogGSC, Warning, TEXT("Failed to get value from [%s::%s]"), *GetNameSafe(SourceObject), *GetterSource.FunctionName.ToString());
		return false;
	}

	return true;
}

bool UGSCSubsystem::SetSettingValueFromString(const UGSCData_SettingDefinition* Definition, FString NewValue)
{
	if (!Definition)
	{
		return false;
	}

	const auto SetterSource{ Definition->GetSetterSource() };
	if (!SetterSource.IsValid())
	{
		UE_LOG(LogGSC, Warning, TEXT("Invalid GetterSource defined in [%s]"), *GetNameSafe(Definition));
		return false;
	}

	auto* SourceObject{ LoadOrGetSettingSourceObject(SetterSource.SettingSourceName) };
	if (!SourceObject)
	{
		UE_LOG(LogGSC, Warning, TEXT("Could not find SourceObject from SettingSourceName(%s) of SetterSource defined in [%s]"),
																	*SetterSource.SettingSourceName.ToString(), *GetNameSafe(Definition));
		return false;
	}

	FCachedPropertyPath DynamicPath{ SetterSource.FunctionName.ToString() };

	if (!PropertyPathHelpers::SetPropertyValueFromString(SourceObject, DynamicPath, NewValue))
	{
		UE_LOG(LogGSC, Warning, TEXT("Failed to get value from [%s::%s]"), *GetNameSafe(SourceObject), *SetterSource.FunctionName.ToString());
		return false;
	}

	return true;
}

#pragma endregion
