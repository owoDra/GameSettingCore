// Copyright (C) 2023 owoDra

#include "GSCFormat_Collection.h"

#include "Data/GSCData_Setting.h"
#include "Data/Filter/GSCFilterBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSCFormat_Collection)


#pragma region CollectionBase

void UGSCCollectionFormatBase::GetSettingDataFiltered(TArray<UGSCData_Setting*>& OutDatas, const UGSCFilterBase* Filter) const
{
	OutDatas.Empty();

	// コレクション内の SettingData をフィルタリング

	for (const auto& Each : SettingDatas)
	{
		if (Each.ToSoftObjectPath().IsValid())
		{
			// SettingData を取得または読み込む

			auto* TestData{ Each.IsValid() ? Each.Get() : Each.LoadSynchronous() };

			// フィルターが有効な場合フィルタリングの結果を、無効な場合は常に true

			const auto bDataAllowed{ Filter ? Filter->FilteringSettingData(this, TestData) : true };

			// 許可された SettingData だった場合はリストに追加
			// また SettingData がコレクションタイプのフォーマットだった場合は そのコレクション内の SettingData もフィルタリングする

			if (bDataAllowed)
			{
				OutDatas.Add(TestData);

				if (auto* CollectionFormat{ TestData->GetFormat<UGSCCollectionFormatBase>() })
				{
					TArray<UGSCData_Setting*> NestedDatas;
					CollectionFormat->GetSettingDataFiltered(NestedDatas, Filter);

					OutDatas.Append(NestedDatas);
				}
			}
		}
	}

}

#pragma endregion


#pragma region CollectionSection

void UGSCFormat_CollectionSection::HandleRefreshSettingRequest()
{
	if (!bShouldBroadcastRefreshRequest)
	{
		return;
	}

	for (const auto& Each : SettingDatas)
	{
		if (Each.IsValid())
		{
			Each->RequestRefreshSetting();
		}
	}
}

#pragma endregion
