// Copyright (C) 2023 owoDra

#include "GSCPropertySourcePickerCustomization.h"

#include "Development/GSCDeveloperSettings.h"
#include "Data/Picker/GSCPicker_SettingSource.h"
#include "Data/Picker/GSCPicker_PropertySource.h"
#include "Development/GSCEditorLogChannels.h"

#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "SSearchableComboBox.h"


const FName FGSCPropertySourcePickerCustomization::NAME_PropertySourcePickerProperty = FGSCPicker_PropertySource::StaticStruct()->GetFName();

TSharedRef<IPropertyTypeCustomization> FGSCPropertySourcePickerCustomization::MakeInstance()
{
	return MakeShareable(new FGSCPropertySourcePickerCustomization());
}

void FGSCPropertySourcePickerCustomization::RegisterToPropertyEditorModule(FPropertyEditorModule& PropertyModule)
{
	PropertyModule.RegisterCustomPropertyTypeLayout(
		FGSCPropertySourcePickerCustomization::NAME_PropertySourcePickerProperty,
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FGSCPropertySourcePickerCustomization::MakeInstance));
}

void FGSCPropertySourcePickerCustomization::UnregisterToPropertyEditorModule(FPropertyEditorModule& PropertyModule)
{
	PropertyModule.UnregisterCustomPropertyTypeLayout(FGSCPropertySourcePickerCustomization::NAME_PropertySourcePickerProperty);
}


FName FGSCPropertySourcePickerCustomization::GetSourceClassPropertyName() const
{
	static const FName NAME_SourceClassPropertyName{ GET_MEMBER_NAME_CHECKED(FGSCPicker_PropertySource, SettingSourceName) };
	return NAME_SourceClassPropertyName;
}

FName FGSCPropertySourcePickerCustomization::GetFunctionNamePropertyName() const
{
	static const FName NAME_FunctionNamePropertyName{ GET_MEMBER_NAME_CHECKED(FGSCPicker_PropertySource, FunctionName) };
	return NAME_FunctionNamePropertyName;
}

bool FGSCPropertySourcePickerCustomization::ShouldStaticFunction() const
{
	return false;
}


void FGSCPropertySourcePickerCustomization::CustomizeSourceClassProperty(IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	// CombBox のデータを初期化する

	InitSuggestSettingSources();

	RefreshSuggestSettingSources();

	// 選択中の SettingSource についてのウィジェットを構築

	FText TextRowText;
	SourceClassPropertyHandle->GetValueAsDisplayText(TextRowText);

	const auto TextRowWidgetRef
	{
		SNew(STextBlock)
			.Text(TextRowText)
	};

	SettingSourceTextWidget = TextRowWidgetRef;

	// 候補リストについてのウィジェットを構築

	const auto SearchableComboBoxRef
	{
		SNew(SSearchableComboBox)
			.OptionsSource(&SuggestSettingSourceOptions)
			.OnGenerateWidget_Lambda(
				[](const TSharedPtr<FString> InItem) -> TSharedRef<SWidget>
				{
					return SNew(STextBlock).Text(FText::FromString(*InItem));
				}
			)
			.OnSelectionChanged(this, &ThisClass::OnSuggestSettingSourceSelected)
			.ContentPadding(2.f)
			.MaxListHeight(200.f)
			.IsEnabled(true)
			.Content()
			[
				TextRowWidgetRef
			]
	};

	SuggestSettingSourcesWidget = SearchableComboBoxRef;

	// SettingSourceName の編集箇所に関するウィジェットを構築する

	ChildBuilder
		.AddCustomRow(SourceClassPropertyHandle->GetPropertyDisplayName())
		.Visibility(EVisibility::Visible)
		.NameContent()
		[
			SNew(STextBlock)
				.Text(SourceClassPropertyHandle->GetPropertyDisplayName())
				.Font(IDetailLayoutBuilder::GetDetailFont())
		]
		.ValueContent()
		[
			SNew(SVerticalBox) + SVerticalBox::Slot()
									.AutoHeight()
									.VAlign(VAlign_Fill)
									.Padding(0.f)
									[
										SearchableComboBoxRef
									]
		];
}

void FGSCPropertySourcePickerCustomization::CustomizeFunctionNameProperty(IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	CacheTemplateMetaKey();

	Super::CustomizeFunctionNameProperty(ChildBuilder, CustomizationUtils);
}


void FGSCPropertySourcePickerCustomization::SetSettingSourceNameValue(FName Value)
{
	if (SourceClassPropertyHandle.IsValid())
	{
		SourceClassPropertyHandle->SetValue(Value);
	}

	if (const auto & TextWidget{ SettingSourceTextWidget.Pin() })
	{
		TextWidget->SetText(FText::FromName(Value));
	}
}

void FGSCPropertySourcePickerCustomization::InitSuggestSettingSources()
{
	if (!NoneSettingSourceOption.IsValid())
	{
		TSharedPtr<FString> NoneStringPtr{ MakeShareable(new FString(FName(NAME_None).ToString())) };
		NoneSettingSourceOption = NoneStringPtr;
		SuggestSettingSourceOptions.EmplaceAt(0, MoveTemp(NoneStringPtr));
	}
}

void FGSCPropertySourcePickerCustomization::ResetSuggestSettingSources()
{
	for (int32 Idx{ SuggestSettingSourceOptions.Num() - 1 }; Idx >= 0; --Idx)
	{
		if (!SuggestSettingSourceOptions.IsValidIndex(Idx))
		{
			continue;
		}

		auto StringPtrIt{ SuggestSettingSourceOptions[Idx] };
		if (StringPtrIt != NoneSettingSourceOption)
		{
			StringPtrIt.Reset();
			SuggestSettingSourceOptions.RemoveAt(Idx);
		}
	}
}

void FGSCPropertySourcePickerCustomization::RefreshSuggestSettingSources()
{
	ResetSuggestSettingSources();

	if (!SourceClassPropertyHandle.IsValid())
	{
		return;
	}

	// デベロッパ設定を読み込み

	const auto* DevSettings{ GetDefault<UGSCDeveloperSettings>() };
	if (!ensureMsgf(DevSettings, TEXT("Can't accese GameSettingsCore's DeveloperSettings")))
	{
		return;
	}

	// 登録された SettingSource の名前から候補リストを作成

	TArray<FName> OutNames;
	DevSettings->SettingSources.GetKeys(OutNames);

	FName SelectedSettingSourceName{ NAME_None };
	SourceClassPropertyHandle->GetValue(SelectedSettingSourceName);

	auto bFoundSpecifiedName{ false };
	for (const auto& Each : OutNames)
	{
		if (SelectedSettingSourceName == Each)
		{
			bFoundSpecifiedName = true;
		}

		SuggestSettingSourceOptions.Emplace(MakeShareable(new FString(Each.ToString())));
	}

	// 既に選択中の SettingSourceName がない場合は None に設定

	if (!bFoundSpecifiedName)
	{
		SetSettingSourceNameValue(NAME_None);
	}
}

void FGSCPropertySourcePickerCustomization::OnSuggestSettingSourceSelected(TSharedPtr<FString> SelectedStringPtr, ESelectInfo::Type SelectInfo)
{
	if (const auto * SelectedString{ SelectedStringPtr.Get() })
	{
		SetSettingSourceNameValue(**SelectedString);
	}
}


bool FGSCPropertySourcePickerCustomization::UpdateTemplateFunction()
{
	// Meta指定子 に変更がない場合はスキップ

	const auto TemplateMetaValue{ FName(ParentPropertyHandle->GetMetaData(TemplateMetaKeyName)) };
	if (TemplateMetaValue == TemplateMetaValueName)
	{
		return true;
	}
	TemplateMetaValueName = TemplateMetaValue;

	// Meta指定子 の値が空だったら TemplateFunction を削除

	if (TemplateMetaValue.IsNone())
	{
		TemplateFunctionWeakPtr.Reset();
		TemplateMetaValueName = NAME_None;
		return true;
	}

	// Meta指定子 の値を クラス名部分 と 関数名部分 で分ける

	static const FString Delimiter{ TEXT("::") };

	TArray<FString> ParsedStrArray;
	TemplateMetaValue.ToString().ParseIntoArray(ParsedStrArray, *Delimiter);

	// クラスを検索

	static constexpr int32 ClassNameIndex{ 0 };

	const FString ClassPathName{ ParsedStrArray.IsValidIndex(ClassNameIndex) ? ParsedStrArray[ClassNameIndex] : FString() };
	const auto* ScopeClass{ !ClassPathName.IsEmpty() ? UClass::TryFindTypeSlow<UClass>(ClassPathName, EFindFirstObjectOptions::ExactClass) : nullptr };
	if (!ensureMsgf(
			ScopeClass
			, TEXT("Unknown ScopeClass (MetaKey: %s, MetaValue: %s, ClassPathName: %s)")
			, *TemplateMetaKeyName.ToString()
			, *TemplateMetaValueName.ToString()
			, *ClassPathName)
		)
	{
		return false;
	}

	// 関数を検索

	static constexpr int32 FunctionNameIndex{ 1 };

	const FString FunctionName{ ParsedStrArray.IsValidIndex(FunctionNameIndex) ? ParsedStrArray[FunctionNameIndex] : FString() };
	auto* TemplateFunction{ !FunctionName.IsEmpty() ? ScopeClass->FindFunctionByName(*FunctionName) : nullptr };
	if (!ensureMsgf(
			TemplateFunction
			, TEXT("Unknown TemplateFunction (MetaKey: %s, MetaValue: %s, FunctionName: %s)")
			, *TemplateMetaKeyName.ToString()
			, *TemplateMetaValueName.ToString()
			, *FunctionName)
		)
	{
		return false;
	}

	TemplateFunctionWeakPtr = TemplateFunction;

	return true;
}

const UClass* FGSCPropertySourcePickerCustomization::GetSelectedSourceClass() const
{
	FName CurrentSettingSourceName{ NAME_None };
	SourceClassPropertyHandle->GetValue(CurrentSettingSourceName);
	if (CurrentSettingSourceName.IsNone())
	{
		return nullptr;
	}

	// デベロッパ設定を読み込み

	const auto* DevSettings{ GetDefault<UGSCDeveloperSettings>() };
	if (!ensureMsgf(DevSettings, TEXT("Can't accese GameSettingsCore's DeveloperSettings")))
	{
		return nullptr;
	}

	// SettingSource の取得

	const auto* SettingSource{ DevSettings->SettingSources.Find(CurrentSettingSourceName) };
	if (!ensureMsgf(SettingSource, TEXT("Invalid SettingSource (FindKey: %s)"), *CurrentSettingSourceName.ToString()))
	{
		return nullptr;
	}

	// クラスの取得

	const auto SoftClassPath{ SettingSource->SourceClass };
	if (!ensureMsgf(SoftClassPath.IsValid(), TEXT("Invalid SourceClass")))
	{
		return nullptr;
	}

	return SoftClassPath.TryLoadClass<UObject>();
}

void FGSCPropertySourcePickerCustomization::CacheTemplateMetaKey()
{
	if (!TemplateMetaKeyName.IsNone() || !ParentPropertyHandle.IsValid())
	{
		return;
	}

	//
	// Meta指定子 のキーを検索するための配列定数
	//
	static const TArray<FName, TFixedAllocator<2>> TemplateMetaKeys
	{
		TEXT("GetterFuncTemplate"),
		TEXT("SetterFuncTemplate")
	};

	// Meta指定子 のキーを持っているかどうか検証

	for (const auto& Each : TemplateMetaKeys)
	{
		if (ParentPropertyHandle->HasMetaData(Each))
		{
			TemplateMetaKeyName = Each;
			break;
		}
	}
}
