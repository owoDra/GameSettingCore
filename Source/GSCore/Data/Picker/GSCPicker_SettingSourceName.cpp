// Copyright (C) 2023 owoDra

#include "GSCPicker_SettingSourceName.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSCPicker_SettingSourceName)

const FGSCPicker_SettingSourceName FGSCPicker_SettingSourceName::Empty = FGSCPicker_SettingSourceName();


bool FGSCPicker_SettingSourceName::IsValid() const
{
    return !Name.IsNone();
}
