// Copyright (C) 2023 owoDra

#include "GSCFormat_Scalar.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSCFormat_Scalar)


#pragma region ScalarBase

bool UGSCScalarPropertyFormatBase::SetPropertyValue(float NewValue)
{
    return SetPropertyValueInternal(GetSetterSource(), FString::SanitizeFloat(NewValue, GetFractionDigits()));
}

bool UGSCScalarPropertyFormatBase::GetPropertyValue(float& OutValue)
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

bool UGSCScalarPropertyFormatBase::GetPropertyDefault(float& OutValue)
{
    OutValue = 0.0;

    FString StringValue;
    auto bSuccess{ GetPropertyDefaultInternal(GetDefaultSource(), StringValue) };

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

#pragma endregion


#pragma region ScalarFloat

const FGSCPicker_PropertySource& UGSCFormat_ScalarFloat::GetGetterSource() const
{
    return GetterSource;
}

const FGSCPicker_PropertySource& UGSCFormat_ScalarFloat::GetSetterSource() const
{
    return SetterSource;
}

const FGSCPicker_PropertySource& UGSCFormat_ScalarFloat::GetDefaultSource() const
{
    return DefaultSource;
}


void UGSCFormat_ScalarFloat::GetValueRange(float& OutMax, float& OutMin) const
{
    OutMax = MaxValue;
    OutMin = MaxValue;
}

float UGSCFormat_ScalarFloat::GetStep() const
{
    return Step;
}

int32 UGSCFormat_ScalarFloat::GetFractionDigits() const
{
    return FractionDigits;
}

#pragma endregion


#pragma region ScalarInt

const FGSCPicker_PropertySource& UGSCFormat_ScalarInt::GetGetterSource() const
{
    return GetterSource;
}

const FGSCPicker_PropertySource& UGSCFormat_ScalarInt::GetSetterSource() const
{
    return SetterSource;
}

const FGSCPicker_PropertySource& UGSCFormat_ScalarInt::GetDefaultSource() const
{
    return DefaultSource;
}


void UGSCFormat_ScalarInt::GetValueRange(float& OutMax, float& OutMin) const
{
    OutMax = static_cast<float>(MaxValue);
    OutMin = static_cast<float>(MinValue);
}

float UGSCFormat_ScalarInt::GetStep() const
{
    return static_cast<float>(Step);
}

int32 UGSCFormat_ScalarInt::GetFractionDigits() const
{
    return 0;
}

#pragma endregion
