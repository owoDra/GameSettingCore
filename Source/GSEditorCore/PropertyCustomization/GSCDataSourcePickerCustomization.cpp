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
	// OuterObject ���擾

	TArray<UObject*> OuterObjects;
	PropertyHandle->GetOuterObjects(OuterObjects);
	PropertyOuterObject = OuterObjects.IsValidIndex(0) ? OuterObjects[0] : nullptr;
	
	// �ϐ��ύX���̃R�[���o�b�N��ݒ�

	ParentPropertyHandle = PropertyHandle;
	auto RefreshSuggestListDelegate{ FSimpleDelegate::CreateSP(this, &ThisClass::RefreshSuggestList) };
	PropertyHandle->SetOnPropertyValueChanged(RefreshSuggestListDelegate);
	PropertyHandle->SetOnChildPropertyValueChanged(RefreshSuggestListDelegate);

	// �ϐ� SourceClass �ɂ��Ă̍��ڂ�ǉ�

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

	// �ϐ� FunctionName �ɂ��Ă̍��ڂ�ǉ�
	
	static const FName NAME_FunctionNamePropertyName{ GET_MEMBER_NAME_CHECKED(FGSCPicker_DataSource, FunctionName) };
	FunctionNameHandle = PropertyHandle->GetChildHandle(NAME_FunctionNamePropertyName);
	if (FunctionNameHandle.IsValid())
	{
		// CombBox �̃f�[�^������������

		InitSuggestList();

		CacheTemplateMetaKey();

		RefreshSuggestList();

		// �I�𒆂̊֐����ɂ��ẴE�B�W�F�b�g���\�z

		FText TextRowText;
		FunctionNameHandle->GetValueAsDisplayText(TextRowText);

		const auto TextRowWidgetRef
		{
			SNew(STextBlock)
				.Text(TextRowText)
		};

		RowTextWidget = TextRowWidgetRef;

		// ��⃊�X�g�ɂ��ẴE�B�W�F�b�g���\�z

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

		// �֐����̕ҏW�ӏ��Ɋւ���E�B�W�F�b�g���\�z����

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

	// SourceClass ���I������Ă��Ȃ��ꍇ�͌�⃊�X�g�𖳌���

	FString ValueString;
	SourceClassHandle->GetValueAsDisplayString(ValueString);
	const auto SelectedSourceClassName{ (ValueString.Len() < NAME_SIZE) ? FName(ValueString) : NAME_None };
	if (SelectedSourceClassName.IsNone())
	{
		UE_LOG(LogGSCEditor, Log, TEXT("Invalidate Suggest List (SourceClass is not selected)"));
		InvalidateSuggestList();
		return;
	}

	// ���ڂɕύX���Ȃ��ꍇ�͍X�V���X�L�b�v����

	const auto bHasClassChanged{ SelectedSourceClassName != PrevSelectedSourceCalssName };
	PrevSelectedSourceCalssName = SelectedSourceClassName;

	const auto bHasTemplateFuncChanged{ UpdateTemplateFunction() };
	if (!bHasClassChanged && !bHasTemplateFuncChanged && !SuggestListOptions.IsEmpty())
	{
		return;
	}

	// �I�������N���X�������������ꍇ�͖���������

	const auto* SelectedSourceClass{ GetSelectedSourceClass() };
	if (!SelectedSourceClass)
	{
		UE_LOG(LogGSCEditor, Log, TEXT("Invalidate Suggest List (SourceClass is not valid)"));
		InvalidateSuggestList();
		return;
	}

	// ��⃊�X�g��L�������A������������

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

	// ���ɑI�𒆂̊֐������Ȃ��ꍇ�� None �ɐݒ�

	if (!bFoundSpecifiedFunction)
	{
		UE_LOG(LogGSCEditor, Log, TEXT("Set Function Name (Not found selected function)"));
		SetFunctionNameValue(NAME_None);
	}

	// ��⃊�X�g���X�V����

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

	// CombBox ���X�V����

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
	// ���؂���֐��ƌ��؂ɗp����֐����L�����m�F����

	if (!Function)
	{
		return false;
	}

	const auto* TemplateFunction{ TemplateFunctionWeakPtr.Get() };
	if (!TemplateFunction)
	{
		// ���ؗp�̊֐����ݒ肳��Ă��Ȃ��ꍇ�͖������� true ��Ԃ�

		return true;
	}

	// �v���p�e�B�������^�C�v�����ؗp�̃����_�֐����쐬

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

	// ���֐��̃v���p�e�B���r���ď����������������؂���

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

		// ���֐��̃v���p�e�B�����s��v�������ꍇ�� false ��Ԃ�

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
	// Meta�w��q �ɕύX���Ȃ��ꍇ�̓X�L�b�v

	const auto TemplateMetaValue{ FName(ParentPropertyHandle->GetMetaData(TemplateMetaKeyName)) };
	if (TemplateMetaValue == TemplateMetaValueName)
	{
		return true;
	}
	TemplateMetaValueName = TemplateMetaValue;

	// Meta�w��q �̒l���󂾂����� TemplateFunction ���폜

	if (TemplateMetaValue.IsNone())
	{
		TemplateFunctionWeakPtr.Reset();
		TemplateMetaValueName = NAME_None;
		return true;
	}

	// Meta�w��q �̒l�� �N���X������ �� �֐������� �ŕ�����

	static const FString Delimiter{ TEXT("::") };

	TArray<FString> ParsedStrArray;
	TemplateMetaValue.ToString().ParseIntoArray(ParsedStrArray, *Delimiter);

	// �N���X������

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

	// �֐�������

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
	// Meta�w��q �̃L�[���������邽�߂̔z��萔
	//
	static const TArray<FName, TFixedAllocator<3>> TemplateMetaKeys
	{
		TEXT("ContextFuncTemplate"),
		TEXT("GetterFuncTemplate"),
		TEXT("SetterFuncTemplate")
	};

	// Meta�w��q �̃L�[�������Ă��邩�ǂ�������

	for (const auto& Each : TemplateMetaKeys)
	{
		if (ParentPropertyHandle->HasMetaData(Each))
		{
			TemplateMetaKeyName = Each;
			break;
		}
	}

	// Static �֐����������邩��ݒ�

	static const FName FunctionContextTemplate{ TEXT("ContextFuncTemplate") };

	bIsStaticFunctionInternal = (TemplateMetaKeyName == FunctionContextTemplate);
}
