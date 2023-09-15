// Copyright (C) 2023 owoDra

#include "GSCFormat_Float.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSCFormat_Float)


const FGSCPicker_PropertySource& UGSCFormat_Float::GetGetterSource() const
{
    return GetterSource;
}

const FGSCPicker_PropertySource& UGSCFormat_Float::GetSetterSource() const
{
    return SetterSource;
}

const FGSCPicker_PropertySource& UGSCFormat_Float::GetDesiredValueSource() const
{
    return DesiredValueSource;
}


bool UGSCFormat_Float::SetPropertyValue(float NewValue)
{
    return SetPropertyValueInternal(GetSetterSource(), FString::SanitizeFloat(NewValue, FractionDigits));
}

bool UGSCFormat_Float::GetPropertyValue(float& OutValue)
{
    OutValue = 0.0;

    FString StringValue;
    auto bSuccess{ GetPropertyValueInternal(GetGetterSource(), StringValue) };

    if (StringValue.IsNumeric())
    {
        OutValue = FCString::Atof(*StringValue);
    }
    else
    {
        bSuccess = false;
    }

    return bSuccess;
}
