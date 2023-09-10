// Copyright (C) 2023 owoDra

#include "GSCDataSourcePickerCustomization.h"

#include "Definition/Picker/GSCPicker_DataSource.h"
#include "Development/GSCEditorLogChannels.h"

#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "SSearchableComboBox.h"


TSharedRef<IPropertyTypeCustomization> FGSCDataSourcePickerCustomization::MakeInstance()
{
	return MakeShareable(new FGSCDataSourcePickerCustomization());
}

void FGSCDataSourcePickerCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	HeaderRow
		.NameContent()
		[
			PropertyHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			PropertyHandle->CreatePropertyValueWidget()
		];
}

void FGSCDataSourcePickerCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	// OuterObject を取得

	TArray<UObject*> OuterObjects;
	PropertyHandle->GetOuterObjects(OuterObjects);
	PropertyOuterObject = OuterObjects.IsValidIndex(0) ? OuterObjects[0] : nullptr;
	
	// 変数変更時のコールバックを設定

	ParentPropertyHandle = PropertyHandle;
	auto RefreshSuggestListDelegate{ FSimpleDelegate::CreateSP(this, &ThisClass::RefreshSuggestList) };
	PropertyHandle->SetOnPropertyValueChanged(RefreshSuggestListDelegate);
	PropertyHandle->SetOnChildPropertyValueChanged(RefreshSuggestListDelegate);

	// 変数 SourceClass についての項目を追加

	static const FName NAME_SourceClassPropertyName{ GET_MEMBER_NAME_CHECKED(FGSCPicker_DataSource, SourceClass) };
	SourceClassHandle = PropertyHandle->GetChildHandle(NAME_SourceClassPropertyName);
	if (SourceClassHandle.IsValid())
	{
		ChildBuilder
			.AddProperty(SourceClassHandle.ToSharedRef())
			.ShouldAutoExpand(true)
			.IsEnabled(true)
			.Visibility(EVisibility::Visible);
	}

	// 変数 FunctionName についての項目を追加
	
	static const FName NAME_FunctionNamePropertyName{ GET_MEMBER_NAME_CHECKED(FGSCPicker_DataSource, FunctionName) };
	FunctionNameHandle = PropertyHandle->GetChildHandle(NAME_FunctionNamePropertyName);
	if (FunctionNameHandle.IsValid())
	{
		// CombBox のデータを初期化する

		InitSuggestList();

		CacheTemplateMetaKey();

		RefreshSuggestList();

		// 選択中の関数名についてのウィジェットを構築

		FText TextRowText;
		FunctionNameHandle->GetValueAsDisplayText(TextRowText);

		const auto TextRowWidgetRef
		{
			SNew(STextBlock)
				.Text(TextRowText)
		};

		RowTextWidget = TextRowWidgetRef;

		// 候補リストについてのウィジェットを構築

		const auto SearchableComboBoxRef
		{
			SNew(SSearchableComboBox)
				.OptionsSource(&SuggestListOptions)
				.OnGenerateWidget_Lambda(
					[](const TSharedPtr<FString> InItem) -> TSharedRef<SWidget>
					{
						return SNew(STextBlock).Text(FText::FromString(*InItem));
					}
				)
				.OnSelectionChanged(this, &ThisClass::OnSuggestSelected)
				.ContentPadding(2.f)
				.MaxListHeight(200.f)
				.IsEnabled(true)
				.Content()
				[
					TextRowWidgetRef
				]
		};

		SuggestListWidget = SearchableComboBoxRef;

		// 関数名の編集箇所に関するウィジェットを構築する

		ChildBuilder
			.AddCustomRow(FunctionNameHandle->GetPropertyDisplayName())
			.Visibility(EVisibility::Visible)
			.NameContent()
			[
				SNew(STextBlock)
					.Text(FunctionNameHandle->GetPropertyDisplayName())
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
}


void FGSCDataSourcePickerCustomization::SetFunctionNameValue(FName Value)
{
	if (FunctionNameHandle.IsValid())
	{
		FunctionNameHandle->SetValue(Value);
	}

	if (const auto& TextWidget{ RowTextWidget.Pin() })
	{
		TextWidget->SetText(FText::FromName(Value));
	}
}

void FGSCDataSourcePickerCustomization::InitSuggestList()
{
	if (!NoneStringOption.IsValid())
	{
		TSharedPtr<FString> NoneStringPtr{ MakeShareable(new FString(FCoreTexts::Get().None.ToString())) };
		NoneStringOption = NoneStringPtr;
		SuggestListOptions.EmplaceAt(0, MoveTemp(NoneStringPtr));
	}
}

void FGSCDataSourcePickerCustomization::ResetSuggestList()
{
	for (int32 Idx{ SuggestListOptions.Num() - 1 }; Idx >= 0; --Idx)
	{
		if (!SuggestListOptions.IsValidIndex(Idx))
		{
			continue;
		}

		auto StringPtrIt{ SuggestListOptions[Idx] };
		if (StringPtrIt != NoneStringOption)
		{
			StringPtrIt.Reset();
			SuggestListOptions.RemoveAt(Idx);
		}
	}
}

void FGSCDataSourcePickerCustomization::RefreshSuggestList()
{
	if (!SourceClassHandle.IsValid() || !FunctionNameHandle.IsValid())
	{
		UE_LOG(LogGSCEditor, Log, TEXT("Invalidate Suggest List (Invalid Child Properties)"));
		InvalidateSuggestList();
		return;
	}

	// SourceClass が選択されていない場合は候補リストを無効化

	FString ValueString;
	SourceClassHandle->GetValueAsDisplayString(ValueString);
	const auto SelectedSourceClassName{ (ValueString.Len() < NAME_SIZE) ? FName(ValueString) : NAME_None };
	if (SelectedSourceClassName.IsNone())
	{
		UE_LOG(LogGSCEditor, Log, TEXT("Invalidate Suggest List (SourceClass is not selected)"));
		InvalidateSuggestList();
		return;
	}

	// 項目に変更がない場合は更新をスキップする

	const auto bHasClassChanged{ SelectedSourceClassName != PrevSelectedSourceCalssName };
	PrevSelectedSourceCalssName = SelectedSourceClassName;

	const auto bHasTemplateFuncChanged{ UpdateTemplateFunction() };
	if (!bHasClassChanged && !bHasTemplateFuncChanged && !SuggestListOptions.IsEmpty())
	{
		return;
	}

	// 選択したクラスが無効だった場合は無効化する

	const auto* SelectedSourceClass{ GetSelectedSourceClass() };
	if (!SelectedSourceClass)
	{
		UE_LOG(LogGSCEditor, Log, TEXT("Invalidate Suggest List (SourceClass is not valid)"));
		InvalidateSuggestList();
		return;
	}

	// 候補リストを有効化し、候補を検索する

	SetSuggestListEnabled(true);
	ResetSuggestList();

	FName SelectedFunctionName{ NAME_None };
	FunctionNameHandle->GetValue(SelectedFunctionName);

	TArray<FName> FoundList;
	auto bFoundSpecifiedFunction{ false };
	for (TFieldIterator<UFunction> It{ SelectedSourceClass, EFieldIteratorFlags::ExcludeSuper }; It; ++It)
	{
		const auto* FunctionIt{ *It };
		if (FunctionIt && FunctionIt != TemplateFunctionWeakPtr &&
			(!bIsStaticFunctionInternal || (FunctionIt->FunctionFlags & FUNC_Static) != 0) && IsSignatureCompatible(FunctionIt))
		{
			auto FunctionItName{ FunctionIt->GetFName() };
			if (FunctionItName == SelectedFunctionName)
			{
				bFoundSpecifiedFunction = true;
			}

			FoundList.AddUnique(MoveTemp(FunctionItName));
		}
	}

	// 既に選択中の関数名がない場合は None に設定

	if (!bFoundSpecifiedFunction)
	{
		UE_LOG(LogGSCEditor, Log, TEXT("Set Function Name (Not found selected function)"));
		SetFunctionNameValue(NAME_None);
	}

	// 候補リストを更新する

	for (const auto& Each : FoundList)
	{
		const auto StringName{ Each.ToString() };
		const auto bAlreadyContains
		{
			SuggestListOptions.ContainsByPredicate(
				[&StringName](const TSharedPtr<FString>& SuggestListOption)
				{
					return SuggestListOption && SuggestListOption->Equals(StringName);
				}
			)
		};

		if (bAlreadyContains)
		{
			continue;
		}

		SuggestListOptions.Emplace(MakeShareable(new FString(StringName)));
	}

	// CombBox を更新する

	if (const auto& SearchableComboBox{ SuggestListWidget.Pin() })
	{
		SearchableComboBox->RefreshOptions();
	}
}

void FGSCDataSourcePickerCustomization::InvalidateSuggestList()
{
	SetSuggestListEnabled(false);

	SetFunctionNameValue(NAME_None);
}

void FGSCDataSourcePickerCustomization::SetSuggestListEnabled(bool bEnabled)
{
	if (const auto& SearchableComboBox{ SuggestListWidget.Pin() })
	{
		SearchableComboBox->SetEnabled(bEnabled);
	}
}

void FGSCDataSourcePickerCustomization::OnSuggestSelected(TSharedPtr<FString> SelectedStringPtr, ESelectInfo::Type SelectInfo)
{
	if (const auto* SelectedString{ SelectedStringPtr.Get() })
	{
		UE_LOG(LogGSCEditor, Log, TEXT("Set Function Name (Suggest function selected)"));
		SetFunctionNameValue(**SelectedString);
	}
}


const UClass* FGSCDataSourcePickerCustomization::GetSelectedSourceClass() const
{
	UObject* ObjectValue{ nullptr };
	SourceClassHandle->GetValue(ObjectValue);
	if (ObjectValue)
	{
		return Cast<UClass>(ObjectValue);
	}

	return nullptr;
}

bool FGSCDataSourcePickerCustomization::IsSignatureCompatible(const UFunction* Function) const
{
	// 検証する関数と検証に用いる関数が有効か確認する

	if (!Function)
	{
		return false;
	}

	const auto* TemplateFunction{ TemplateFunctionWeakPtr.Get() };
	if (!TemplateFunction)
	{
		// 検証用の関数が設定されていない場合は無条件で true を返す

		return true;
	}

	// プロパティが同じタイプか検証用のラムダ関数を作成

	auto ArePropertiesTheSame
	{ 
		[](const FProperty* A, const FProperty* B)
		{
			if (A == B)
			{
				return true;
			}

			if (!A || !B)
			{
				return false;
			}

			if (A->GetSize() != B->GetSize())
			{
				return false;
			}

			if (A->GetOffset_ForGC() != B->GetOffset_ForGC())
			{
				return false;
			}

			if (!A->SameType(B))
			{
				if (A->PropertyFlags & B->PropertyFlags & CPF_ReturnParm && A->IsA(B->GetClass()))
				{
					return true;
				}
				return false;
			}

			return true;
		} 
	};

	// 両関数のプロパティを比較して署名が等しいか検証する

	const uint64 IgnoreFlags{ CPF_ReturnParm | UFunction::GetDefaultIgnoredSignatureCompatibilityFlags() };

	TFieldIterator<FProperty> IteratorA{ TemplateFunction };
	TFieldIterator<FProperty> IteratorB{ Function };

	while (IteratorA && (IteratorA->PropertyFlags & CPF_Parm))
	{
		if (IteratorB && (IteratorB->PropertyFlags & CPF_Parm))
		{
			const FProperty* PropA = *IteratorA;
			const FProperty* PropB = *IteratorB;

			const uint64 PropertyMash{ PropA->PropertyFlags ^ PropB->PropertyFlags };
			if (!ArePropertiesTheSame(PropA, PropB) || ((PropertyMash & ~IgnoreFlags) != 0))
			{
				return false;
			}
		}

		// 両関数のプロパティ数が不一致だった場合は false を返す

		else
		{
			return false;
		}

		++IteratorA;
		++IteratorB;
	}

	return true;
}

bool FGSCDataSourcePickerCustomization::UpdateTemplateFunction()
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

void FGSCDataSourcePickerCustomization::CacheTemplateMetaKey()
{
	if (!TemplateMetaKeyName.IsNone() || !ParentPropertyHandle.IsValid())
	{
		return;
	}

	//
	// Meta指定子 のキーを検索するための配列定数
	//
	static const TArray<FName, TFixedAllocator<3>> TemplateMetaKeys
	{
		TEXT("ContextFuncTemplate"),
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

	// Static 関数を検索するかを設定

	static const FName FunctionContextTemplate{ TEXT("ContextFuncTemplate") };

	bIsStaticFunctionInternal = (TemplateMetaKeyName == FunctionContextTemplate);
}
