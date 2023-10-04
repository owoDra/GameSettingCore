// Copyright (C) 2023 owoDra

#include "GSCSettingSourceNamePickerCustomization.h"

#include "Data/Picker/GSCPicker_SettingSourceName.h"
#include "Development/GSCDeveloperSettings.h"
#include "Development/GSCEditorLogChannels.h"

#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "SSearchableComboBox.h"

const FName FGSCSettingSourceNamePickerCustomization::NAME_SettingSourceNamePickerProperty = FGSCPicker_SettingSourceName::StaticStruct()->GetFName();


TSharedRef<IPropertyTypeCustomization> FGSCSettingSourceNamePickerCustomization::MakeInstance()
{
	return MakeShareable(new FGSCSettingSourceNamePickerCustomization());
}

void FGSCSettingSourceNamePickerCustomization::RegisterToPropertyEditorModule(FPropertyEditorModule& PropertyModule)
{
	PropertyModule.RegisterCustomPropertyTypeLayout(
		FGSCSettingSourceNamePickerCustomization::NAME_SettingSourceNamePickerProperty,
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FGSCSettingSourceNamePickerCustomization::MakeInstance));
}

void FGSCSettingSourceNamePickerCustomization::UnregisterToPropertyEditorModule(FPropertyEditorModule& PropertyModule)
{
	PropertyModule.UnregisterCustomPropertyTypeLayout(FGSCSettingSourceNamePickerCustomization::NAME_SettingSourceNamePickerProperty);
}


void FGSCSettingSourceNamePickerCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	/*HeaderRow
		.NameContent()
		[
			PropertyHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			PropertyHandle->CreatePropertyValueWidget()
		];*/

	// �ϐ��ύX���̃R�[���o�b�N��ݒ�

	ParentPropertyHandle = PropertyHandle;

	// �ϐ� SourceClass �ɂ��Ă̍��ڂ�ǉ�

	SourceNamePropertyHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FGSCPicker_SettingSourceName, Name));
	if (SourceNamePropertyHandle.IsValid())
	{
		// CombBox �̃f�[�^������������

		InitSuggestList();

		RefreshSuggestList();

		// �I�𒆂̊֐����ɂ��ẴE�B�W�F�b�g���\�z

		FText TextRowText;
		SourceNamePropertyHandle->GetValueAsDisplayText(TextRowText);

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

		HeaderRow
			.NameContent()
			[
				SNew(STextBlock)
					.Text(ParentPropertyHandle->GetPropertyDisplayName())
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

void FGSCSettingSourceNamePickerCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	//// �ϐ��ύX���̃R�[���o�b�N��ݒ�

	//ParentPropertyHandle = PropertyHandle;

	//// �ϐ� SourceClass �ɂ��Ă̍��ڂ�ǉ�

	//SourceNamePropertyHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FGSCPicker_SettingSourceName, Name));
	//if (SourceNamePropertyHandle.IsValid())
	//{
	//	// CombBox �̃f�[�^������������

	//	InitSuggestList();

	//	RefreshSuggestList();

	//	// �I�𒆂̊֐����ɂ��ẴE�B�W�F�b�g���\�z

	//	FText TextRowText;
	//	SourceNamePropertyHandle->GetValueAsDisplayText(TextRowText);

	//	const auto TextRowWidgetRef
	//	{
	//		SNew(STextBlock)
	//			.Text(TextRowText)
	//	};

	//	RowTextWidget = TextRowWidgetRef;

	//	// ��⃊�X�g�ɂ��ẴE�B�W�F�b�g���\�z

	//	const auto SearchableComboBoxRef
	//	{
	//		SNew(SSearchableComboBox)
	//			.OptionsSource(&SuggestListOptions)
	//			.OnGenerateWidget_Lambda(
	//				[](const TSharedPtr<FString> InItem) -> TSharedRef<SWidget>
	//				{
	//					return SNew(STextBlock).Text(FText::FromString(*InItem));
	//				}
	//			)
	//			.OnSelectionChanged(this, &ThisClass::OnSuggestSelected)
	//			.ContentPadding(2.f)
	//			.MaxListHeight(200.f)
	//			.IsEnabled(true)
	//			.Content()
	//			[
	//				TextRowWidgetRef
	//			]
	//	};

	//	SuggestListWidget = SearchableComboBoxRef;

	//	// �֐����̕ҏW�ӏ��Ɋւ���E�B�W�F�b�g���\�z����

	//	ChildBuilder
	//		.AddCustomRow(SourceNamePropertyHandle->GetPropertyDisplayName())
	//		.Visibility(EVisibility::Visible)
	//		.NameContent()
	//		[
	//			SNew(STextBlock)
	//				.Text(SourceNamePropertyHandle->GetPropertyDisplayName())
	//				.Font(IDetailLayoutBuilder::GetDetailFont())
	//		]
	//		.ValueContent()
	//		[
	//			SNew(SVerticalBox) + SVerticalBox::Slot()
	//									.AutoHeight()
	//									.VAlign(VAlign_Fill)
	//									.Padding(0.f)
	//									[
	//										SearchableComboBoxRef
	//									]
	//		];
	//}
}


void FGSCSettingSourceNamePickerCustomization::SetSettingSourceNameValue(FName Value)
{
	if (SourceNamePropertyHandle.IsValid())
	{
		SourceNamePropertyHandle->SetValue(Value);
	}

	if (const auto & TextWidget{ RowTextWidget.Pin() })
	{
		TextWidget->SetText(FText::FromName(Value));
	}
}

void FGSCSettingSourceNamePickerCustomization::InitSuggestList()
{
	if (!NoneStringOption.IsValid())
	{
		TSharedPtr<FString> NoneStringPtr{ MakeShareable(new FString(FName(NAME_None).ToString())) };
		NoneStringOption = NoneStringPtr;
		SuggestListOptions.EmplaceAt(0, MoveTemp(NoneStringPtr));
	}
}

void FGSCSettingSourceNamePickerCustomization::ResetSuggestList()
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

void FGSCSettingSourceNamePickerCustomization::RefreshSuggestList()
{
	ResetSuggestList();

	if (!SourceNamePropertyHandle.IsValid())
	{
		return;
	}

	// �f�x���b�p�ݒ��ǂݍ���

	const auto* DevSettings{ GetDefault<UGSCDeveloperSettings>() };
	if (!ensureMsgf(DevSettings, TEXT("Can't accese GameSettingsCore's DeveloperSettings")))
	{
		return;
	}

	// �o�^���ꂽ SettingSource �̖��O�����⃊�X�g���쐬

	TArray<FName> OutNames;
	DevSettings->SettingSources.GetKeys(OutNames);

	FName SelectedSettingSourceName{ NAME_None };
	SourceNamePropertyHandle->GetValue(SelectedSettingSourceName);

	auto bFoundSpecifiedName{ false };
	for (const auto& Each : OutNames)
	{
		if (SelectedSettingSourceName == Each)
		{
			bFoundSpecifiedName = true;
		}

		SuggestListOptions.Emplace(MakeShareable(new FString(Each.ToString())));
	}

	// ���ɑI�𒆂� SettingSourceName ���Ȃ��ꍇ�� None �ɐݒ�

	if (!bFoundSpecifiedName)
	{
		SetSettingSourceNameValue(NAME_None);
	}
}

void FGSCSettingSourceNamePickerCustomization::OnSuggestSelected(TSharedPtr<FString> SelectedStringPtr, ESelectInfo::Type SelectInfo)
{
	if (const auto* SelectedString{ SelectedStringPtr.Get() })
	{
		SetSettingSourceNameValue(**SelectedString);
	}
}
