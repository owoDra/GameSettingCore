// Copyright (C) 2023 owoDra

#include "GSCData_ListVisual.h"

#include "Definition/GSCData_SettingCollection.h"
#include "Definition/GSCData_SettingDefinition.h"
#include "Definition/Format/GSCFormatBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSCData_ListVisual)


TSubclassOf<UUserWidget> UGSCData_ListVisual::GetEntryWidgetForData(const UGSCData_SettingBase* InData)
{
	if (!InData)
	{
		return nullptr;
	}

	// SettingCollection だった場合はヘッダーを参照

	if (const auto* Collection{ Cast<UGSCData_SettingCollection>(InData) })
	{
		return EntryWidgetForHeader.WidgetClass;
	}

	// SettingDefinition だった場合は設定を検索

	if (const auto* Definition{ Cast<UGSCData_SettingDefinition>(InData) })
	{
		// SettingTag で検索

		const auto SettingTag{ Definition->SettingTag };
		if (const auto* EntryWidget{ EntryWidgetsForTag.Find(SettingTag) })
		{
			return EntryWidget->WidgetClass;
		}

		// Format で検索

		if (auto Format{ Definition->Format })
		{
			TSubclassOf<UGSCFormatBase> FormatClass{ Format->GetClass() };
			if (const auto* EntryWidget{ EntryWidgetsForFormat.Find(FormatClass) })
			{
				return EntryWidget->WidgetClass;
			}
		}
	}

	return nullptr;
}
