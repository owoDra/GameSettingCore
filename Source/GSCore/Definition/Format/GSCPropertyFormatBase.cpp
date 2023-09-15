// Copyright (C) 2023 owoDra

#include "GSCPropertyFormatBase.h"

#include "Subsystem/GSCSubsystem.h"
#include "Definition/GSCData_Setting.h"
#include "Development/GSCLogChannels.h"

#include "PropertyPathHelpers.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSCPropertyFormatBase)


void UGSCPropertyFormatBase::HandleRefreshSettingRequest()
{
	TrySetPropertyToDesiredValue();
}

void UGSCPropertyFormatBase::HandleSettingValueChanged()
{
	TryRefreshRelatedSettings();
}


bool UGSCPropertyFormatBase::SetPropertyValueInternal(const FGSCPicker_PropertySource& SetterSource, FString NewValue)
{
	if (SetterSource.IsValid() && GEngine && GEngine->IsInitialized())
	{
		auto* Subsystem{ GEngine->GetEngineSubsystem<UGSCSubsystem>() };

		auto* SourceObject{ Subsystem->LoadOrGetSettingSourceObject(SetterSource.SettingSourceName) };
		if (!SourceObject)
		{
			UE_LOG(LogGSC, Error, TEXT("Could not find SourceObject from SettingSourceName(%s) of SetterSource defined in [%s]"),
				*SetterSource.SettingSourceName.ToString(), *GetNameSafe(GetOuter()));
			return false;
		}

		FCachedPropertyPath DynamicPath{ SetterSource.FunctionName.ToString() };
		if (!PropertyPathHelpers::SetPropertyValueFromString(SourceObject, DynamicPath, NewValue))
		{
			UE_LOG(LogGSC, Error, TEXT("Failed to set value from [%s::%s]"), *GetNameSafe(SourceObject), *SetterSource.FunctionName.ToString());
			return false;
		}
		else
		{
			HandleSettingValueChanged();
			return true;
		}
	}

	return false;
}

bool UGSCPropertyFormatBase::GetPropertyValueInternal(const FGSCPicker_PropertySource& GetterSource, FString& OutValue)
{
	OutValue = FString();

	if (GetterSource.IsValid() && GEngine && GEngine->IsInitialized())
	{
		auto* Subsystem{ GEngine->GetEngineSubsystem<UGSCSubsystem>() };

		auto* SourceObject{ Subsystem->LoadOrGetSettingSourceObject(GetterSource.SettingSourceName) };
		if (!SourceObject)
		{
			UE_LOG(LogGSC, Error, TEXT("Could not find SourceObject from SettingSourceName(%s) of GetterSource defined in [%s]"),
				*GetterSource.SettingSourceName.ToString(), *GetNameSafe(GetOuter()));
			return false;
		}

		FCachedPropertyPath DynamicPath{ GetterSource.FunctionName.ToString() };
		if (!PropertyPathHelpers::GetPropertyValueAsString(SourceObject, DynamicPath, OutValue))
		{
			UE_LOG(LogGSC, Error, TEXT("Failed to get value from [%s::%s]"), *GetNameSafe(SourceObject), *GetterSource.FunctionName.ToString());
			return false;
		}
		else
		{
			return true;
		}
	}

	return false;
}


void UGSCPropertyFormatBase::TryRefreshRelatedSettings()
{
	for (const auto& SettingData : RelatedSettings)
	{
		if (SettingData)
		{
			SettingData->RequestRefreshSetting();
		}
	}
}

void UGSCPropertyFormatBase::TrySetPropertyToDesiredValue()
{
	FString DesiredValue;
	if (GetPropertyValueInternal(GetDesiredValueSource(), DesiredValue))
	{
		SetPropertyValueInternal(GetSetterSource(), DesiredValue);
	}
}
