// Copyright (C) 2023 owoDra

#include "GSCData_SettingDefinition.h"

#include "Definition/Format/GSCFormatBase.h"
#include "Definition/Picker/GSCPicker_PropertySource.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSCData_SettingDefinition)


UGSCData_SettingDefinition::UGSCData_SettingDefinition(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}


FGSCPicker_PropertySource UGSCData_SettingDefinition::GetGetterSource() const
{
    if (!Format)
    {
        return FGSCPicker_PropertySource::Empty;
    }

    return Format->GetGetterSource();
}

FGSCPicker_PropertySource UGSCData_SettingDefinition::GetSetterSource() const
{
    if (!Format)
    {
        return FGSCPicker_PropertySource::Empty;
    }

    return Format->GetSetterSource();
}
