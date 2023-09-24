// Copyright (C) 2023 owoDra

#include "GSCPropertyFormatBase.h"

#include "Subsystem/GSCSubsystem.h"
#include "Definition/GSCData_Setting.h"
#include "Development/GSCLogChannels.h"

#include "PropertyPathHelpers.h"

#if WITH_EDITOR

#include "Misc/DataValidation.h"

#endif // WITH_EDITOR

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSCPropertyFormatBase)


#if WITH_EDITOR

#define LOCTEXT_NAMESPACE "PropertyFormatBase"

EDataValidationResult UGSCPropertyFormatBase::IsDataValid(FDataValidationContext& Context)
{
	auto Result{ CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid) };
	auto DataName{ FText::FromString(GetNameSafe(GetOuter())) };

	if (!GetGetterSource().IsValid())
	{
		Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);
		Context.AddError(FText::Format(
			LOCTEXT("InvalidGetterSource", "GetterSource is not set in [{0}]"), DataName));
	}

	if (!GetSetterSource().IsValid())
	{
		Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);
		Context.AddError(FText::Format(
			LOCTEXT("InvalidSetterSource", "SetterSource is not set in [{0}]"), DataName));
	}

	if (!GetDefaultSource().IsValid())
	{
		Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);
		Context.AddError(FText::Format(
			LOCTEXT("InvalidDefaultSource", "DefaultSource is not set in [{0}]"), DataName));
	}

	return Result;
}

#undef LOCTEXT_NAMESPACE

#endif // WITH_EDITOR


void UGSCPropertyFormatBase::HandleRefreshSettingRequest()
{
	OnRequestRefreshSetting.Broadcast(true);
}

void UGSCPropertyFormatBase::HandleSettingValueChanged()
{
	OnRequestRefreshSetting.Broadcast(false);

	TryRefreshRelatedSettings();
}


bool UGSCPropertyFormatBase::SetPropertyValueInternal(const FGSCPicker_PropertySource& SetterSource, FString NewValue)
{
	if (auto* Subsystem{ UGSCSubsystem::Get() })
	{
		if (Subsystem->SetSettingValueFromString(GetOuter(), SetterSource, NewValue))
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

	if (auto* Subsystem{ UGSCSubsystem::Get() })
	{
		return Subsystem->GetSettingValueAsString(GetOuter(), GetterSource, OutValue);
	}

	return false;
}

bool UGSCPropertyFormatBase::GetPropertyDefaultInternal(const FGSCPicker_PropertySource& DefaultSource, FString& OutValue)
{
	OutValue = FString();

	if (auto* Subsystem{ UGSCSubsystem::Get() })
	{
		return Subsystem->GetSettingDefaultAsString(GetOuter(), DefaultSource, OutValue);
	}

	return false;
}

bool UGSCPropertyFormatBase::SetPropertyToDefault()
{
	if (auto* Subsystem{ UGSCSubsystem::Get() })
	{
		return Subsystem->SetSettingValueToDefault(GetOuter(), GetDefaultSource(), GetSetterSource());
	}

	return false;
}


void UGSCPropertyFormatBase::TryRefreshRelatedSettings()
{
	for (const auto& SettingData : RelatedSettings)
	{
		if (SettingData.IsValid())
		{
			SettingData->RequestRefreshSetting();
		}
	}
}
