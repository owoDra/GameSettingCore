// Copyright (C) 2023 owoDra

#include "GSCFormat_Collection.h"

#include "Data/GSCData_Setting.h"
#include "Data/Filter/GSCFilterBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSCFormat_Collection)


#pragma region CollectionBase

void UGSCCollectionFormatBase::GetSettingDataFiltered(TArray<UGSCData_Setting*>& OutDatas, const UGSCFilterBase* Filter) const
{
	OutDatas.Empty();

	// �R���N�V�������� SettingData ���t�B���^�����O

	for (const auto& Each : SettingDatas)
	{
		if (Each.ToSoftObjectPath().IsValid())
		{
			// SettingData ���擾�܂��͓ǂݍ���

			auto* TestData{ Each.IsValid() ? Each.Get() : Each.LoadSynchronous() };

			// �t�B���^�[���L���ȏꍇ�t�B���^�����O�̌��ʂ��A�����ȏꍇ�͏�� true

			const auto bDataAllowed{ Filter ? Filter->FilteringSettingData(this, TestData) : true };

			// �����ꂽ SettingData �������ꍇ�̓��X�g�ɒǉ�
			// �܂� SettingData ���R���N�V�����^�C�v�̃t�H�[�}�b�g�������ꍇ�� ���̃R���N�V�������� SettingData ���t�B���^�����O����

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
