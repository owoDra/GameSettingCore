// Copyright (C) 2023 owoDra

#include "GSCListView.h"

#include "Definition/GSCData_SettingBase.h"
#include "Definition/GSCData_SettingDefinition.h"
#include "UserInterface/GSCData_ListVisual.h"
#include "UserInterface/GSCListEntryWidgetInterface.h"

#include "Templates/SubclassOf.h"
#include "Blueprint/UserWidget.h"

#if WITH_EDITOR
#include "Editor/WidgetCompilerLog.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSCListView)


#define LOCTEXT_NAMESPACE "UGSCListView"

UGSCListView::UGSCListView(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

#if WITH_EDITOR

void UGSCListView::ValidateCompiledDefaults(IWidgetCompilerLog& InCompileLog) const
{
	Super::ValidateCompiledDefaults(InCompileLog);

	if (!VisualData)
	{
		InCompileLog.Error(FText::Format(FText::FromString("{0} has no VisualData defined."), FText::FromString(GetNameSafe(this))));
	}
}

#endif

UUserWidget& UGSCListView::OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (const auto* Data{ Cast<UGSCData_SettingBase>(Item) })
	{
		if (auto DesiredEntryWidgetClass{ GetEntryWidgetForData(Data) })
		{
			UUserWidget& EntryWidget{ GenerateTypedEntry(DesiredEntryWidgetClass, OwnerTable) };

			if (!IsDesignTime())
			{
				if (auto* Interface{ Cast<IGSCListEntryWidgetInterface>(&EntryWidget) })
				{
					Interface->RegisterData(Data);
				}
			}

			return EntryWidget;
		}
	}

	return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
}

bool UGSCListView::OnIsSelectableOrNavigableInternal(UObject* SelectedItem)
{
	if (SelectedItem->IsA<UGSCData_SettingDefinition>())
	{
		return true;
	}

	return false;
}

TSubclassOf<UUserWidget> UGSCListView::GetEntryWidgetForData(const UGSCData_SettingBase* Data)
{
	if (VisualData)
	{
		return VisualData->GetEntryWidgetForData(Data);
	}

	return nullptr;
}

#undef LOCTEXT_NAMESPACE
