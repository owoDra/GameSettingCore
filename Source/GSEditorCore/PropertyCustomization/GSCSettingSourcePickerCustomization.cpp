// Copyright (C) 2023 owoDra

#include "GSCSettingSourcePickerCustomization.h"

#include "Data/Picker/GSCPicker_SettingSource.h"
#include "Development/GSCEditorLogChannels.h"

#include "Modules/ModuleManager.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"


const FName FGSCSettingSourcePickerCustomization::NAME_SettingSourcePickerProperty = FGSCPicker_SettingSource::StaticStruct()->GetFName();

TSharedRef<IPropertyTypeCustomization> FGSCSettingSourcePickerCustomization::MakeInstance()
{
	return MakeShareable(new FGSCSettingSourcePickerCustomization());
}

void FGSCSettingSourcePickerCustomization::RegisterToPropertyEditorModule(FPropertyEditorModule& PropertyModule)
{
	PropertyModule.RegisterCustomPropertyTypeLayout(
		FGSCSettingSourcePickerCustomization::NAME_SettingSourcePickerProperty,
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FGSCSettingSourcePickerCustomization::MakeInstance));
}

void FGSCSettingSourcePickerCustomization::UnregisterToPropertyEditorModule(FPropertyEditorModule& PropertyModule)
{
	PropertyModule.UnregisterCustomPropertyTypeLayout(FGSCSettingSourcePickerCustomization::NAME_SettingSourcePickerProperty);
}


FName FGSCSettingSourcePickerCustomization::GetSourceClassPropertyName() const
{
	static const FName NAME_SourceClassPropertyName{ GET_MEMBER_NAME_CHECKED(FGSCPicker_SettingSource, SourceClass) };
	return NAME_SourceClassPropertyName;
}

FName FGSCSettingSourcePickerCustomization::GetFunctionNamePropertyName() const
{
	static const FName NAME_FunctionNamePropertyName{ GET_MEMBER_NAME_CHECKED(FGSCPicker_SettingSource, FunctionName) };
	return NAME_FunctionNamePropertyName;
}

bool FGSCSettingSourcePickerCustomization::ShouldStaticFunction() const
{
	return true;
}


bool FGSCSettingSourcePickerCustomization::UpdateTemplateFunction()
{
	static const FName FunctionName{ TEXT("StaticContextTemplate__DelegateSignature") };

	auto* TemplateFunction{ UGSCPickerTemplate_SettingSource::StaticClass()->FindFunctionByName(FunctionName) };

	TemplateFunctionWeakPtr = TemplateFunction;

	if (!ensureMsgf(TemplateFunction, TEXT("Invalid TemplateFunction")))
	{
		return false;
	}

	return true;
}

const UClass* FGSCSettingSourcePickerCustomization::GetSelectedSourceClass() const
{
	FString StringValue;
	SourceClassPropertyHandle->GetValueAsDisplayString(StringValue);
	FSoftClassPath SoftClassPath{ StringValue };

	if (!ensureMsgf(SoftClassPath.IsValid(), TEXT("Invalid SourceClass (Value: %s)"), *StringValue))
	{
		return nullptr;
	}

	return SoftClassPath.TryLoadClass<UObject>();
}
