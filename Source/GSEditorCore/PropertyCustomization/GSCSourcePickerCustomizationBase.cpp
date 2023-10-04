// Copyright (C) 2023 owoDra

#include "GSCSourcePickerCustomizationBase.h"

#include "Development/GSCEditorLogChannels.h"

#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "SSearchableComboBox.h"


void FGSCSourcePickerCustomizationBase::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
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

void FGSCSourcePickerCustomizationBase::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	// 変数変更時のコールバックを設定

	ParentPropertyHandle = PropertyHandle;
	auto RefreshSuggestListDelegate{ FSimpleDelegate::CreateSP(this, &ThisClass::HandlePropertyChange) };
	PropertyHandle->SetOnPropertyValueChanged(RefreshSuggestListDelegate);
	PropertyHandle->SetOnChildPropertyValueChanged(RefreshSuggestListDelegate);

	// 変数 SourceClass についての項目を追加

	SourceClassPropertyHandle = PropertyHandle->GetChildHandle(GetSourceClassPropertyName());
	if (SourceClassPropertyHandle.IsValid())
	{
		CustomizeSourceClassProperty(ChildBuilder, CustomizationUtils);
	}

	// 変数 FunctionName についての項目を追加
	
	FunctionNamePropertyHandle = PropertyHandle->GetChildHandle(GetFunctionNamePropertyName());
	if (SourceClassPropertyHandle.IsValid())
	{
		CustomizeFunctionNameProperty(ChildBuilder, CustomizationUtils);
	}
}

void FGSCSourcePickerCustomizationBase::CustomizeSourceClassProperty(IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	ChildBuilder
		.AddProperty(SourceClassPropertyHandle.ToSharedRef())
		.ShouldAutoExpand(true)
		.IsEnabled(true)
		.Visibility(EVisibility::Visible);
}

void FGSCSourcePickerCustomizationBase::CustomizeFunctionNameProperty(IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	// CombBox のデータを初期化する

	InitSuggestList();

	RefreshSuggestList();

	// 選択中の関数名についてのウィジェットを構築

	FText TextRowText;
	FunctionNamePropertyHandle->GetValueAsDisplayText(TextRowText);

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
		.AddCustomRow(FunctionNamePropertyHandle->GetPropertyDisplayName())
		.Visibility(EVisibility::Visible)
		.NameContent()
		[
			SNew(STextBlock)
				.Text(FunctionNamePropertyHandle->GetPropertyDisplayName())
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

void FGSCSourcePickerCustomizationBase::HandlePropertyChange()
{
	RefreshSuggestList();
}


void FGSCSourcePickerCustomizationBase::SetFunctionNameValue(FName Value)
{
	if (FunctionNamePropertyHandle.IsValid())
	{
		FunctionNamePropertyHandle->SetValue(Value);
	}

	if (const auto& TextWidget{ RowTextWidget.Pin() })
	{
		TextWidget->SetText(FText::FromName(Value));
	}
}

void FGSCSourcePickerCustomizationBase::InitSuggestList()
{
	if (!NoneStringOption.IsValid())
	{
		TSharedPtr<FString> NoneStringPtr{ MakeShareable(new FString(FName(NAME_None).ToString())) };
		NoneStringOption = NoneStringPtr;
		SuggestListOptions.EmplaceAt(0, MoveTemp(NoneStringPtr));
	}
}

void FGSCSourcePickerCustomizationBase::ResetSuggestList()
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

void FGSCSourcePickerCustomizationBase::RefreshSuggestList()
{
	if (!SourceClassPropertyHandle.IsValid() || !FunctionNamePropertyHandle.IsValid())
	{
		InvalidateSuggestList();
		return;
	}

	// SourceClass が選択されていない場合は候補リストを無効化

	FString SelectedSourceClassName;
	SourceClassPropertyHandle->GetValueAsDisplayString(SelectedSourceClassName);
	if (SelectedSourceClassName.IsEmpty())
	{
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
		InvalidateSuggestList();
		return;
	}

	// 候補リストを有効化し、候補を検索する

	ResetSuggestList();

	FName SelectedFunctionName{ NAME_None };
	FunctionNamePropertyHandle->GetValue(SelectedFunctionName);

	TArray<FName> FoundList;
	auto bFoundSpecifiedFunction{ false };
	for (TFieldIterator<UFunction> It{ SelectedSourceClass, EFieldIteratorFlags::IncludeSuper }; It; ++It)
	{
		const auto* FunctionIt{ *It };
		if (FunctionIt && FunctionIt != TemplateFunctionWeakPtr &&
			(!ShouldStaticFunction() || (FunctionIt->FunctionFlags & FUNC_Static) != 0) && IsSignatureCompatible(FunctionIt))
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

void FGSCSourcePickerCustomizationBase::InvalidateSuggestList()
{
	ResetSuggestList();

	SetFunctionNameValue(NAME_None);
}

void FGSCSourcePickerCustomizationBase::OnSuggestSelected(TSharedPtr<FString> SelectedStringPtr, ESelectInfo::Type SelectInfo)
{
	if (const auto* SelectedString{ SelectedStringPtr.Get() })
	{
		SetFunctionNameValue(**SelectedString);
	}
}


const UClass* FGSCSourcePickerCustomizationBase::GetSelectedSourceClass() const
{
	UObject* ObjectValue{ nullptr };
	SourceClassPropertyHandle->GetValue(ObjectValue);
	if (ObjectValue)
	{
		return Cast<UClass>(ObjectValue);
	}

	return nullptr;
}

bool FGSCSourcePickerCustomizationBase::IsSignatureCompatible(const UFunction* Function) const
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
