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
	// �ϐ��ύX���̃R�[���o�b�N��ݒ�

	ParentPropertyHandle = PropertyHandle;
	auto RefreshSuggestListDelegate{ FSimpleDelegate::CreateSP(this, &ThisClass::HandlePropertyChange) };
	PropertyHandle->SetOnPropertyValueChanged(RefreshSuggestListDelegate);
	PropertyHandle->SetOnChildPropertyValueChanged(RefreshSuggestListDelegate);

	// �ϐ� SourceClass �ɂ��Ă̍��ڂ�ǉ�

	SourceClassPropertyHandle = PropertyHandle->GetChildHandle(GetSourceClassPropertyName());
	if (SourceClassPropertyHandle.IsValid())
	{
		CustomizeSourceClassProperty(ChildBuilder, CustomizationUtils);
	}

	// �ϐ� FunctionName �ɂ��Ă̍��ڂ�ǉ�
	
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
	// CombBox �̃f�[�^������������

	InitSuggestList();

	RefreshSuggestList();

	// �I�𒆂̊֐����ɂ��ẴE�B�W�F�b�g���\�z

	FText TextRowText;
	FunctionNamePropertyHandle->GetValueAsDisplayText(TextRowText);

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

	// SourceClass ���I������Ă��Ȃ��ꍇ�͌�⃊�X�g�𖳌���

	FString SelectedSourceClassName;
	SourceClassPropertyHandle->GetValueAsDisplayString(SelectedSourceClassName);
	if (SelectedSourceClassName.IsEmpty())
	{
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
		InvalidateSuggestList();
		return;
	}

	// ��⃊�X�g��L�������A������������

	ResetSuggestList();

	FName SelectedFunctionName{ NAME_None };
	FunctionNamePropertyHandle->GetValue(SelectedFunctionName);

	TArray<FName> FoundList;
	auto bFoundSpecifiedFunction{ false };
	for (TFieldIterator<UFunction> It{ SelectedSourceClass, EFieldIteratorFlags::ExcludeSuper }; It; ++It)
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

	// ���ɑI�𒆂̊֐������Ȃ��ꍇ�� None �ɐݒ�

	if (!bFoundSpecifiedFunction)
	{
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
