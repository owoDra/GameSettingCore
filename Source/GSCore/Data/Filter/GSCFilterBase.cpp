// Copyright (C) 2023 owoDra

#include "GSCFilterBase.h"

#include "Data/GSCData_Setting.h"
#include "Data/Format/GSCFormat_Collection.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSCFilterBase)


bool UGSCFilterBase::FilteringSettingData_Implementation(const UGSCCollectionFormatBase* CollectionFormat, const UGSCData_Setting* TestData) const
{
	if (!bAllowDeprecated && TestData->IsDeprecated())
	{
		return false;
	}

	return true;
}
