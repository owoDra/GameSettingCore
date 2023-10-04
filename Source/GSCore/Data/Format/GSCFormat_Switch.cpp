// Copyright (C) 2023 owoDra

#include "GSCFormat_Switch.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSCFormat_Switch)


#pragma region SwitchBase

#define LOCTEXT_NAMESPACE "FormatSwitchBase"

UGSCSwitchPropertyFormatBase::UGSCSwitchPropertyFormatBase()
{
    TrueText = LOCTEXT("TrueText", "ON");
    FalseText = LOCTEXT("FalseText", "OFF");
}


bool UGSCSwitchPropertyFormatBase::SetPropertyValue(bool NewValue)
{
    const FString StringValue{ NewValue ? TEXT("true") : TEXT("false") };
    return SetPropertyValueInternal(StringValue);
}

bool UGSCSwitchPropertyFormatBase::GetPropertyValue(bool& OutValue)
{
    OutValue = false;

    FString StringValue;
    auto bSuccess{ GetPropertyValueInternal(StringValue) };

    OutValue = StringValue.ToBool();

    return bSuccess;
}

bool UGSCSwitchPropertyFormatBase::GetPropertyDefault(bool& OutValue)
{
    OutValue = false;

    FString StringValue;
    auto bSuccess{ GetPropertyDefaultInternal(StringValue) };

    OutValue = StringValue.ToBool();

    return bSuccess;
}

#undef LOCTEXT_NAMESPACE

#pragma endregion


#pragma region SwitchBool

const FGSCPicker_PropertySource& UGSCFormat_SwitchBool::GetGetterSource() const
{
    return GetterSource;
}

const FGSCPicker_PropertySource& UGSCFormat_SwitchBool::GetSetterSource() const
{
    return SetterSource;
}

const FGSCPicker_PropertySource& UGSCFormat_SwitchBool::GetDefaultSource() const
{
    return GetterSource;
}

#pragma endregion
