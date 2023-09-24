// Copyright (C) 2023 owoDra

#include "GSCEditCondition_DataSource.h"

#include "Subsystem/GSCSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSCEditCondition_DataSource)


FGSCEditableState UGSCEditCondition_DataSource::GatherEditableState_Implementation()
{
	auto Result{ bHideWhenDisabled ? FGSCEditableState::Collapsed : FGSCEditableState::NotEditable };

	if (GEngine && GEngine->IsInitialized() && EditConditionSource.IsValid())
	{
		auto* Subsystem{ GEngine->GetEngineSubsystem<UGSCSubsystem>() };

		FString StringValue;
		if (Subsystem->GetSettingValueAsString(GetOuter(), EditConditionSource, StringValue))
		{
			if (StringValue.ToBool())
			{
				Result = FGSCEditableState::Editable;
			}
		}
	}

	return Result;
}
