// Copyright (C) 2023 owoDra

#include "GSCSubsystem.h"

#include "Development/GSCDeveloperSettings.h"
#include "Development/GSCLogChannels.h"
#include "Subsystem/GSCSettingSourceInterface.h"
#include "Definition/GSCData_Setting.h"
#include "Definition/Format/GSCPropertyFormatBase.h"

#include "PropertyPathHelpers.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSCSubsystem)


#pragma region Utilities

UGSCSubsystem* UGSCSubsystem::Get()
{
	if (GEngine && GEngine->IsInitialized())
	{
		return GEngine->GetEngineSubsystem<UGSCSubsystem>();
	}

	return nullptr;
}

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

bool UGSCSubsystem::IsValidSettingSourceName(FName SettingSourceName) const
{
	if (CachedSettingSources.Contains(SettingSourceName))
	{
		return true;
	}

	const auto* Setting{ GetDefault<UGSCDeveloperSettings>() };

	return Setting->SettingSources.Contains(SettingSourceName);
}

#pragma endregion


#pragma region SettingDirtyState

bool UGSCSubsystem::IsDirty(FName SettingSourceName) const
{
	return DirtySettingSources.Contains(SettingSourceName);
}

bool UGSCSubsystem::ContainDirty(const TArray<FName>& SettingSourceName) const
{
	for (const auto& Each : SettingSourceName)
	{
		if (IsDirty(Each))
		{
			return true;
		}
	}

	return false;
}

bool UGSCSubsystem::ContainDirty(const TArray<FGSCPicker_SettingSourceName>& SettingSourceName) const
{
	for (const auto& Each : SettingSourceName)
	{
		if (IsDirty(Each.Name))
		{
			return true;
		}
	}

	return false;
}

void UGSCSubsystem::MarkDirty(FName SettingSourceName)
{
	auto bAlreadySet{ false };
	DirtySettingSources.Add(SettingSourceName, &bAlreadySet);

	if (!bAlreadySet)
	{
		OnSettingSourceDirtyChanged.Broadcast(SettingSourceName, true);
	}
}

void UGSCSubsystem::ClearDirty(FName SettingSourceName)
{
	DirtySettingSources.Remove(SettingSourceName);

	OnSettingSourceDirtyChanged.Broadcast(SettingSourceName, false);
}

#pragma endregion


#pragma region SettingSourceRequests

bool UGSCSubsystem::RequestApplySetting(FName SettingSourceName)
{
	auto* SourceObject{ LoadOrGetSettingSourceObject(SettingSourceName) };
	if (!ensureMsgf(SourceObject, TEXT("Request made when SettingSource(%s) was not ready"), *SettingSourceName.ToString()))
	{
		return false;
	}

	if (!ensureMsgf(SourceObject->Implements<UGSCSettingSourceInterface>(), TEXT("%s does not implement IGSCSettingSourceInterface"), *GetNameSafe(SourceObject)))
	{
		return false;
	}

	if (!IGSCSettingSourceInterface::Execute_RequestApplySettings(SourceObject))
	{
		return false;
	}

	ClearDirty(SettingSourceName);

	OnSettingSourceApplied.Broadcast(SettingSourceName);

	return true;
}

bool UGSCSubsystem::RequestApplySettings(const TArray<FName>& SettingSourceNames)
{
	if (SettingSourceNames.IsEmpty())
	{
		return false;
	}

	for (const auto& Each : SettingSourceNames)
	{
		RequestApplySetting(Each);
	}

	return true;
}

bool UGSCSubsystem::RequestApplySettings(const TArray<FGSCPicker_SettingSourceName>& SettingSourceNames)
{
	if (SettingSourceNames.IsEmpty())
	{
		return false;
	}

	for (const auto& Each : SettingSourceNames)
	{
		RequestApplySetting(Each.Name);
	}

	return true;
}


bool UGSCSubsystem::RequestResetSetting(FName SettingSourceName)
{
	auto* SourceObject{ LoadOrGetSettingSourceObject(SettingSourceName) };
	if (!ensureMsgf(SourceObject, TEXT("Request made when SettingSource(%s) was not ready"), *SettingSourceName.ToString()))
	{
		return false;
	}

	if (!ensureMsgf(SourceObject->Implements<UGSCSettingSourceInterface>(), TEXT("%s does not implement IGSCSettingSourceInterface"), *GetNameSafe(SourceObject)))
	{
		return false;
	}

	if (!IGSCSettingSourceInterface::Execute_RequestResetSettings(SourceObject))
	{
		return false;
	}

	MarkDirty(SettingSourceName);

	OnSettingSourceReset.Broadcast(SettingSourceName);

	return true;
}

bool UGSCSubsystem::RequestResetSettings(const TArray<FName>& SettingSourceNames)
{
	if (SettingSourceNames.IsEmpty())
	{
		return false;
	}

	for (const auto& Each : SettingSourceNames)
	{
		RequestResetSetting(Each);
	}

	return true;
}

bool UGSCSubsystem::RequestResetSettings(const TArray<FGSCPicker_SettingSourceName>& SettingSourceNames)
{
	if (SettingSourceNames.IsEmpty())
	{
		return false;
	}

	for (const auto& Each : SettingSourceNames)
	{
		RequestResetSetting(Each.Name);
	}

	return true;
}


bool UGSCSubsystem::RequestReloadSetting(FName SettingSourceName)
{
	auto* SourceObject{ LoadOrGetSettingSourceObject(SettingSourceName) };
	if (!ensureMsgf(SourceObject, TEXT("Request made when SettingSource(%s) was not ready"), *SettingSourceName.ToString()))
	{
		return false;
	}

	if (!ensureMsgf(SourceObject->Implements<UGSCSettingSourceInterface>(), TEXT("%s does not implement IGSCSettingSourceInterface"), *GetNameSafe(SourceObject)))
	{
		return false;
	}

	if (!IGSCSettingSourceInterface::Execute_RequestReloadSettings(SourceObject))
	{
		return false;
	}

	ClearDirty(SettingSourceName);

	OnSettingSourceReloaded.Broadcast(SettingSourceName);

	return true;
}

bool UGSCSubsystem::RequestReloadSettings(const TArray<FName>& SettingSourceNames)
{
	if (SettingSourceNames.IsEmpty())
	{
		return false;
	}

	for (const auto& Each : SettingSourceNames)
	{
		RequestReloadSetting(Each);
	}

	return true;
}

bool UGSCSubsystem::RequestReloadSettings(const TArray<FGSCPicker_SettingSourceName>& SettingSourceNames)
{
	if (SettingSourceNames.IsEmpty())
	{
		return false;
	}

	for (const auto& Each : SettingSourceNames)
	{
		RequestReloadSetting(Each.Name);
	}

	return true;
}

#pragma endregion


#pragma region SettingProperties

bool UGSCSubsystem::GetSettingValueAsString(UGSCPropertyFormatBase* PropertyFormat, FString& OutValue)
{
	OutValue = FString();

	if (!PropertyFormat)
	{
		return false;
	}

	const auto& GetterSource{ PropertyFormat->GetGetterSource() };
	if (!GetterSource.IsValid())
	{
		return false;
	}

	auto* SourceObject{ LoadOrGetSettingSourceObject(GetterSource.SettingSourceName) };
	if (!SourceObject)
	{
		UE_LOG(LogGSC, Warning, TEXT("Could not find SourceObject from SettingSourceName(%s) of GetterSource defined in [%s]"),
			*GetterSource.SettingSourceName.ToString(), *GetNameSafe(PropertyFormat->GetOuter()));
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

bool UGSCSubsystem::GetSettingValueAsString(const UObject* Context, const FGSCPicker_PropertySource& GetterSource, FString& OutValue)
{
	OutValue = FString();

	if (!GetterSource.IsValid())
	{
		return false;
	}

	auto* SourceObject{ LoadOrGetSettingSourceObject(GetterSource.SettingSourceName) };
	if (!SourceObject)
	{
		UE_LOG(LogGSC, Warning, TEXT("Could not find SourceObject from SettingSourceName(%s) of GetterSource defined in [%s]"),
			*GetterSource.SettingSourceName.ToString(), *GetNameSafe(Context));
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

bool UGSCSubsystem::SetSettingValueFromString(UGSCPropertyFormatBase* PropertyFormat, FString NewValue)
{
	if (!PropertyFormat)
	{
		return false;
	}

	const auto& SetterSource{ PropertyFormat->GetSetterSource() };
	if (!SetterSource.IsValid())
	{
		return false;
	}

	auto* SourceObject{ LoadOrGetSettingSourceObject(SetterSource.SettingSourceName) };
	if (!SourceObject)
	{
		UE_LOG(LogGSC, Error, TEXT("Could not find SourceObject from SettingSourceName(%s) of SetterSource defined in [%s]"),
			*SetterSource.SettingSourceName.ToString(), *GetNameSafe(PropertyFormat->GetOuter()));
		return false;
	}

	FCachedPropertyPath DynamicPath{ SetterSource.FunctionName.ToString() };

	if (!PropertyPathHelpers::SetPropertyValueFromString(SourceObject, DynamicPath, NewValue))
	{
		UE_LOG(LogGSC, Error, TEXT("Failed to get value from [%s::%s]"), *GetNameSafe(SourceObject), *SetterSource.FunctionName.ToString());
		return false;
	}

	MarkDirty(SetterSource.SettingSourceName);
	PropertyFormat->HandleSettingValueChanged();

	return true;
}

bool UGSCSubsystem::GetSettingDefaultAsString(UGSCPropertyFormatBase* PropertyFormat, FString& OutValue)
{
	if (!PropertyFormat)
	{
		return false;
	}

	const auto& DefaultSource{ PropertyFormat->GetDefaultSource() };
	if (!DefaultSource.IsValid())
	{
		return false;
	}

	auto* SourceObject{ LoadOrGetSettingSourceObject(DefaultSource.SettingSourceName) };
	if (!SourceObject)
	{
		UE_LOG(LogGSC, Warning, TEXT("Could not find SourceObject from SettingSourceName(%s) of DefaultSource defined in [%s]"),
			*DefaultSource.SettingSourceName.ToString(), *GetNameSafe(PropertyFormat->GetOuter()));
		return false;
	}

	auto* SourceObjectCDO{ GetMutableDefault<UObject>(SourceObject->GetClass()) };
	FCachedPropertyPath DynamicPath{ DefaultSource.FunctionName.ToString() };

	if (!PropertyPathHelpers::GetPropertyValueAsString(SourceObjectCDO, DynamicPath, OutValue))
	{
		UE_LOG(LogGSC, Warning, TEXT("Failed to get value from [%s::%s]"), *GetNameSafe(SourceObjectCDO), *DefaultSource.FunctionName.ToString());
		return false;
	}

	return true;
}

bool UGSCSubsystem::GetSettingDefaultAsString(const UObject* Context, const FGSCPicker_PropertySource& DefaultSource, FString& OutValue)
{
	if (!DefaultSource.IsValid())
	{
		return false;
	}

	auto* SourceObject{ LoadOrGetSettingSourceObject(DefaultSource.SettingSourceName) };
	if (!SourceObject)
	{
		UE_LOG(LogGSC, Warning, TEXT("Could not find SourceObject from SettingSourceName(%s) of DefaultSource defined in [%s]"),
			*DefaultSource.SettingSourceName.ToString(), *GetNameSafe(Context));
		return false;
	}

	auto* SourceObjectCDO{ GetMutableDefault<UObject>(SourceObject->GetClass()) };
	FCachedPropertyPath DynamicPath{ DefaultSource.FunctionName.ToString() };

	if (!PropertyPathHelpers::GetPropertyValueAsString(SourceObjectCDO, DynamicPath, OutValue))
	{
		UE_LOG(LogGSC, Warning, TEXT("Failed to get value from [%s::%s]"), *GetNameSafe(SourceObjectCDO), *DefaultSource.FunctionName.ToString());
		return false;
	}

	return true;
}

bool UGSCSubsystem::SetSettingValueToDefault(UGSCPropertyFormatBase* PropertyFormat)
{
	FString StringValue{ FString() };

	if (GetSettingDefaultAsString(PropertyFormat, StringValue))
	{
		return SetSettingValueFromString(PropertyFormat, StringValue);
	}

	return false;
}

#pragma endregion
