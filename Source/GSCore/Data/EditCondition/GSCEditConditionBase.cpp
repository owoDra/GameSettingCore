// Copyright (C) 2023 owoDra

#include "GSCEditConditionBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSCEditConditionBase)


void UGSCEditConditionBase::HandleRefreshSettingRequest()
{
	OnRequestRefreshEditableState.Broadcast(GatherEditableState());
}
