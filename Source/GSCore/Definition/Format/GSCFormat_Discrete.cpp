// Copyright (C) 2023 owoDra

#include "GSCFormat_Discrete.h"

#include "Subsystem/GSCSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSCFormat_Discrete)


#pragma region DiscreteBase

TArray<FText> UGSCDiscretePropertyFormatBase::GetOptionNames() const
{
	auto StringOptions{ GetOptionStrings() };

	TArray<FText> OptionNames;
	for (const auto& Each : StringOptions)
	{
		if (const auto* NameOverride{ OptionNameOverrides.Find(Each) })
		{
			OptionNames.Add(*NameOverride);
		}
		else
		{
			OptionNames.Add(FText::FromString(Each));
		}
	}

	return OptionNames;
}

bool UGSCDiscretePropertyFormatBase::CanUseDynamicOptionSource() const
{
	return GetDynamicOptionSource().IsValid() && GEngine && GEngine->IsInitialized();
}

#pragma endregion


#pragma region DiscreteInt

const FGSCPicker_PropertySource& UGSCFormat_DiscreteInt::GetGetterSource() const
{
	return GetterSource;
}

const FGSCPicker_PropertySource& UGSCFormat_DiscreteInt::GetSetterSource() const
{
	return SetterSource;
}

const FGSCPicker_PropertySource& UGSCFormat_DiscreteInt::GetDefaultSource() const
{
	return DefaultSource;
}

const FGSCPicker_PropertySource& UGSCFormat_DiscreteInt::GetDynamicOptionSource() const
{
	return DynamicOptionSource;
}


bool UGSCFormat_DiscreteInt::SetPropertyOption(int32 NewOptionIdx)
{
	TArray<int32> ValueArray;
	if (!GetDynamicIntOptions(ValueArray))
	{
		Options.GenerateValueArray(ValueArray);
	}

	if (ValueArray.IsValidIndex(NewOptionIdx))
	{
		return SetPropertyValueInternal(GetSetterSource(), FString::FromInt(ValueArray[NewOptionIdx]));
	}

	return false;
}

bool UGSCFormat_DiscreteInt::GetPropertyOption(int32& OutOptionIdx)
{
	OutOptionIdx = INDEX_NONE;

	// プロパティの値を取得

	FString StringValue;
	if (!GetPropertyValueInternal(GetGetterSource(), StringValue))
	{
		return false;
	}

	if (!StringValue.IsNumeric())
	{
		return false;
	}

	auto IntValue{ FCString::Atoi(*StringValue) };

	// 候補の配列を取得

	TArray<int32> ValueArray;
	if (!GetDynamicIntOptions(ValueArray))
	{
		Options.GenerateValueArray(ValueArray);
	}

	// 選択項目を検索

	return ValueArray.Find(IntValue, OutOptionIdx);
}

bool UGSCFormat_DiscreteInt::GetPropertyDefaultOption(int32& OutOptionIdx)
{
	OutOptionIdx = INDEX_NONE;

	// プロパティの値を取得

	FString StringValue;
	if (!GetPropertyDefaultInternal(GetDefaultSource(), StringValue))
	{
		return false;
	}

	if (!StringValue.IsNumeric())
	{
		return false;
	}

	auto IntValue{ FCString::Atoi(*StringValue) };

	// 候補の配列を取得

	TArray<int32> ValueArray;
	if (!GetDynamicIntOptions(ValueArray))
	{
		Options.GenerateValueArray(ValueArray);
	}

	// 選択項目を検索

	return ValueArray.Find(IntValue, OutOptionIdx);
}


bool UGSCFormat_DiscreteInt::GetDynamicOptionStrings(TArray<FString>& OutOptions) const
{
	OutOptions.Empty();

	TArray<int32> IntOptions;
	if (GetDynamicIntOptions(IntOptions))
	{
		for (const auto& Each : IntOptions)
		{
			OutOptions.Add(FString::FromInt(Each));
		}

		return true;
	}

	return false;
}

TArray<FString> UGSCFormat_DiscreteInt::GetOptionStrings() const
{
	TArray<FString> OutOptions;
	if (!GetDynamicOptionStrings(OutOptions))
	{
		Options.GetKeys(OutOptions);
	}
	return OutOptions;
}

bool UGSCFormat_DiscreteInt::GetDynamicIntOptions(TArray<int32>& OutOptions) const
{
	OutOptions.Empty();

	if (CanUseDynamicOptionSource())
	{
		auto* Subsystem{ GEngine->GetEngineSubsystem<UGSCSubsystem>() };
		auto* SourceObject{ Subsystem->LoadOrGetSettingSourceObject(GetDynamicOptionSource().SettingSourceName) };
		if (SourceObject)
		{
			UGSCPickerTemplate_FormatDiscrete::FIntDynamicOptionsTemplate Delegate;
			Delegate.BindUFunction(SourceObject, GetDynamicOptionSource().FunctionName);
			if (Delegate.IsBound())
			{
				Delegate.Execute(OutOptions);

				return true;
			}
		}
	}

	return false;
}

#pragma endregion


#pragma region DiscreteFloat

const FGSCPicker_PropertySource& UGSCFormat_DiscreteFloat::GetGetterSource() const
{
	return GetterSource;
}

const FGSCPicker_PropertySource& UGSCFormat_DiscreteFloat::GetSetterSource() const
{
	return SetterSource;
}

const FGSCPicker_PropertySource& UGSCFormat_DiscreteFloat::GetDefaultSource() const
{
	return DefaultSource;
}

const FGSCPicker_PropertySource& UGSCFormat_DiscreteFloat::GetDynamicOptionSource() const
{
	return DynamicOptionSource;
}


bool UGSCFormat_DiscreteFloat::SetPropertyOption(int32 NewOptionIdx)
{
	TArray<float> ValueArray;
	if (!GetDynamicFloatOptions(ValueArray))
	{
		Options.GenerateValueArray(ValueArray);
	}

	if (ValueArray.IsValidIndex(NewOptionIdx))
	{
		return SetPropertyValueInternal(GetSetterSource(), FString::SanitizeFloat(ValueArray[NewOptionIdx]));
	}

	return false;
}

bool UGSCFormat_DiscreteFloat::GetPropertyOption(int32& OutOptionIdx)
{
	OutOptionIdx = INDEX_NONE;

	// プロパティの値を取得

	FString StringValue;
	if (!GetPropertyValueInternal(GetGetterSource(), StringValue))
	{
		return false;
	}

	if (!StringValue.IsNumeric())
	{
		return false;
	}

	auto FloatValue{ FCString::Atof(*StringValue) };

	// 候補の配列を取得

	TArray<float> ValueArray;
	if (!GetDynamicFloatOptions(ValueArray))
	{
		Options.GenerateValueArray(ValueArray);
	}

	// 選択項目を検索

	return ValueArray.Find(FloatValue, OutOptionIdx);
}

bool UGSCFormat_DiscreteFloat::GetPropertyDefaultOption(int32& OutOptionIdx)
{
	OutOptionIdx = INDEX_NONE;

	// プロパティの値を取得

	FString StringValue;
	if (!GetPropertyDefaultInternal(GetDefaultSource(), StringValue))
	{
		return false;
	}

	if (!StringValue.IsNumeric())
	{
		return false;
	}

	auto FloatValue{ FCString::Atof(*StringValue) };

	// 候補の配列を取得

	TArray<float> ValueArray;
	if (!GetDynamicFloatOptions(ValueArray))
	{
		Options.GenerateValueArray(ValueArray);
	}

	// 選択項目を検索

	return ValueArray.Find(FloatValue, OutOptionIdx);
}


bool UGSCFormat_DiscreteFloat::GetDynamicOptionStrings(TArray<FString>& OutOptions) const
{
	OutOptions.Empty();

	TArray<float> FloatOptions;
	if (GetDynamicFloatOptions(FloatOptions))
	{
		for (const auto& Each : FloatOptions)
		{
			OutOptions.Add(FString::SanitizeFloat(Each));
		}

		return true;
	}

	return false;
}

TArray<FString> UGSCFormat_DiscreteFloat::GetOptionStrings() const
{
	TArray<FString> OutOptions;
	if (!GetDynamicOptionStrings(OutOptions))
	{
		Options.GetKeys(OutOptions);
	}
	return OutOptions;
}

bool UGSCFormat_DiscreteFloat::GetDynamicFloatOptions(TArray<float>& OutOptions) const
{
	OutOptions.Empty();

	if (CanUseDynamicOptionSource())
	{
		auto* Subsystem{ GEngine->GetEngineSubsystem<UGSCSubsystem>() };
		auto* SourceObject{ Subsystem->LoadOrGetSettingSourceObject(GetDynamicOptionSource().SettingSourceName) };
		if (SourceObject)
		{
			UGSCPickerTemplate_FormatDiscrete::FFloatDynamicOptionsTemplate Delegate;
			Delegate.BindUFunction(SourceObject, GetDynamicOptionSource().FunctionName);
			if (Delegate.IsBound())
			{
				Delegate.Execute(OutOptions);

				return true;
			}
		}
	}

	return false;
}

#pragma endregion


#pragma region DiscreteString

const FGSCPicker_PropertySource& UGSCFormat_DiscreteString::GetGetterSource() const
{
	return GetterSource;
}

const FGSCPicker_PropertySource& UGSCFormat_DiscreteString::GetSetterSource() const
{
	return SetterSource;
}

const FGSCPicker_PropertySource& UGSCFormat_DiscreteString::GetDefaultSource() const
{
	return DefaultSource;
}

const FGSCPicker_PropertySource& UGSCFormat_DiscreteString::GetDynamicOptionSource() const
{
	return DynamicOptionSource;
}


bool UGSCFormat_DiscreteString::SetPropertyOption(int32 NewOptionIdx)
{
	TArray<FString> ValueArray;
	if (!GetDynamicOptionStrings(ValueArray))
	{
		ValueArray = Options;
	}

	if (ValueArray.IsValidIndex(NewOptionIdx))
	{
		return SetPropertyValueInternal(GetSetterSource(), ValueArray[NewOptionIdx]);
	}

	return false;
}

bool UGSCFormat_DiscreteString::GetPropertyOption(int32& OutOptionIdx)
{
	OutOptionIdx = INDEX_NONE;

	// プロパティの値を取得

	FString StringValue;
	if (!GetPropertyValueInternal(GetGetterSource(), StringValue))
	{
		return false;
	}

	// 候補の配列を取得

	TArray<FString> ValueArray;
	if (!GetDynamicOptionStrings(ValueArray))
	{
		ValueArray = Options;
	}

	// 選択項目を検索

	return ValueArray.Find(StringValue, OutOptionIdx);
}

bool UGSCFormat_DiscreteString::GetPropertyDefaultOption(int32& OutOptionIdx)
{
	OutOptionIdx = INDEX_NONE;

	// プロパティの値を取得

	FString StringValue;
	if (!GetPropertyDefaultInternal(GetDefaultSource(), StringValue))
	{
		return false;
	}

	// 候補の配列を取得

	TArray<FString> ValueArray;
	if (!GetDynamicOptionStrings(ValueArray))
	{
		ValueArray = Options;
	}

	// 選択項目を検索

	return ValueArray.Find(StringValue, OutOptionIdx);
}


bool UGSCFormat_DiscreteString::GetDynamicOptionStrings(TArray<FString>& OutOptions) const
{
	OutOptions.Empty();

	if (CanUseDynamicOptionSource())
	{
		auto* Subsystem{ GEngine->GetEngineSubsystem<UGSCSubsystem>() };
		auto* SourceObject{ Subsystem->LoadOrGetSettingSourceObject(GetDynamicOptionSource().SettingSourceName) };
		if (SourceObject)
		{
			UGSCPickerTemplate_FormatDiscrete::FStringDynamicOptionsTemplate Delegate;
			Delegate.BindUFunction(SourceObject, GetDynamicOptionSource().FunctionName);
			if (Delegate.IsBound())
			{
				Delegate.Execute(OutOptions);

				return true;
			}
		}
	}

	return false;
}

TArray<FString> UGSCFormat_DiscreteString::GetOptionStrings() const
{
	TArray<FString> OutOptions;
	if (!GetDynamicOptionStrings(OutOptions))
	{
		OutOptions = Options;
	}
	return OutOptions;
}

#pragma endregion
