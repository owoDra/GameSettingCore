// Copyright (C) 2023 owoDra

#include "GSCAssetTypeAction_SettingData.h"

#include "GSEditorCore.h"
#include "Data/GSCData_Setting.h"
#include "Data/Format/GSCFormatBase.h"

#include "Modules/ModuleManager.h"
#include "ClassViewerModule.h"
#include "ClassViewerFilter.h"
#include "AssetRegistry/AssetData.h"
#include "Kismet2/SClassPickerDialog.h"
#include "Kismet2/KismetEditorUtilities.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSCAssetTypeAction_SettingData)


#pragma region AssetClassParentFilter

/**
 * EditorFactories.cpp で定義されている物のコピー
 * EditorFactories.cpp を include できないためここで同じものを定義する
 */
class FAssetClassParentFilter : public IClassViewerFilter
{
public:
	FAssetClassParentFilter()
		: DisallowedClassFlags(CLASS_None), bDisallowBlueprintBase(false)
	{}

	/** All children of these classes will be included unless filtered out by another setting. */
	TSet< const UClass* > AllowedChildrenOfClasses;

	/** Disallowed class flags. */
	EClassFlags DisallowedClassFlags;

	/** Disallow blueprint base classes. */
	bool bDisallowBlueprintBase;

	virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override
	{
		bool bAllowed= !InClass->HasAnyClassFlags(DisallowedClassFlags)
			&& InClass->CanCreateAssetOfClass()
			&& InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InClass) != EFilterReturn::Failed;

		if (bAllowed && bDisallowBlueprintBase)
		{
			if (FKismetEditorUtilities::CanCreateBlueprintOfClass(InClass))
			{
				return false;
			}
		}

		return bAllowed;
	}

	virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const TSharedRef< const IUnloadedBlueprintData > InUnloadedClassData, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override
	{
		if (bDisallowBlueprintBase)
		{
			return false;
		}

		return !InUnloadedClassData->HasAnyClassFlags(DisallowedClassFlags)
			&& InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InUnloadedClassData) != EFilterReturn::Failed;
	}
};

#pragma endregion


#define LOCTEXT_NAMESPACE "AssetTypeActions"

#pragma region SettingDataFactroy

UGSCSettingDataFactory::UGSCSettingDataFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UGSCData_Setting::StaticClass();
}


FText UGSCSettingDataFactory::GetDisplayName() const
{
	return LOCTEXT("SettingData", "Setting Data");
}

bool UGSCSettingDataFactory::DoesSupportClass(UClass* Class)
{
	return Class && Class->IsChildOf(GetSupportedClass());
}

bool UGSCSettingDataFactory::ConfigureProperties()
{
	FormatClass = nullptr;

	// クラスビューを呼び出しフォーマットクラスを選択する

	auto& ClassViewerModule{ FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer") };

	FClassViewerInitializationOptions Options;
	Options.Mode = EClassViewerMode::ClassPicker;

	TSharedPtr<FAssetClassParentFilter> Filter = MakeShareable(new FAssetClassParentFilter);
	Options.ClassFilters.Add(Filter.ToSharedRef());

	Filter->DisallowedClassFlags = CLASS_Abstract | CLASS_Deprecated | CLASS_NewerVersionExists | CLASS_HideDropDown;
	Filter->AllowedChildrenOfClasses.Add(UGSCFormatBase::StaticClass());

	const FText TitleText = LOCTEXT("CreateSettingDataOption", "Pick ForamtClass For SettingData Instance");
	UClass* ChosenClass = nullptr;
	const bool bPressedOk = SClassPickerDialog::PickClass(TitleText, Options, ChosenClass, UDataAsset::StaticClass());

	if (bPressedOk)
	{
		FormatClass = ChosenClass;
	}

	return bPressedOk;
}


UObject* UGSCSettingDataFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	check(InClass->IsChildOf(GetSupportedClass()));

	if (FormatClass)
	{
		auto* NewInstance{ NewObject<UGSCData_Setting>(InParent, InClass, InName, Flags | RF_Transactional) };

#if WITH_EDITOR
		NewInstance->SetFormatClass(FormatClass);
#endif // WITH_EDITOR

		return NewInstance;
	}
	else
	{
		return NewObject<UGSCData_Setting>(InParent, InClass, InName, Flags);
	}
}

#pragma endregion


#pragma region AssetTypeAction

FText FGSCAssetTypeActions_SettingData::GetName() const
{
	return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_SettingData", "Setting Data");
}

FText FGSCAssetTypeActions_SettingData::GetAssetDescription(const FAssetData& AssetData) const
{
	return LOCTEXT("SettingDataAssetDescription", "Data asset that define user accessible game setting option.");
}

FColor FGSCAssetTypeActions_SettingData::GetTypeColor() const
{
	return FColor::Cyan;
}

uint32 FGSCAssetTypeActions_SettingData::GetCategories()
{
	return FGSEditorCoreModule::SettingsCategory;
}

UClass* FGSCAssetTypeActions_SettingData::GetSupportedClass() const
{
	return UGSCData_Setting::StaticClass();
}

#pragma endregion

#undef LOCTEXT_NAMESPACE


