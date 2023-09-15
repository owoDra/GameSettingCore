// Copyright (C) 2023 owoDra

#include "GSCData_Setting.h"

#include "Definition/Format/GSCFormatBase.h"
#include "Definition/EditCondition/GSCEditCondition.h"

#if WITH_EDITOR

#include "Definition/Format/GSCPropertyFormatBase.h"

#include "Misc/DataValidation.h"

#endif // WITH_EDITOR

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSCData_Setting)


#define LOCTEXT_NAMESPACE "UGSCData_Setting"

UGSCData_Setting::UGSCData_Setting(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

#if WITH_EDITOR

EDataValidationResult UGSCData_Setting::IsDataValid(FDataValidationContext& Context)
{
	auto Result{ CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid) };

	if (!Format)
	{
		Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);
		Context.AddError(FText::Format(
			LOCTEXT("InvalidFormat", "Invalid Format defined in [{0}]"), this));
	}
	else
	{
		if (auto* PropertyFormat{ Cast<UGSCPropertyFormatBase>(Format) })
		{
			if (!PropertyFormat->GetGetterSource().IsValid())
			{
				Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);
				Context.AddError(FText::Format(
					LOCTEXT("InvalidGetterSource", "Invalid GetterSource defined in [{0}]"), this));
			}

			if (!PropertyFormat->GetSetterSource().IsValid())
			{
				Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);
				Context.AddError(FText::Format(
					LOCTEXT("InvalidSetterSource", "Invalid SetterSource defined in [{0}]"), this));
			}
		}
	}
	

	return Result;
}

#endif // WITH_EDITOR


void UGSCData_Setting::RequestRefreshSetting()
{
	if (Format)
	{
		Format->HandleRefreshSettingRequest();
	}

	OnRequestRefreshSetting.Broadcast();
}


FGSCEditableState UGSCData_Setting::GetEditableState() const
{
	if (IsDeprecated())
	{
		return FGSCEditableState::Collapsed;
	}

	if (EditCondition)
	{
		return EditCondition->GatherEditableState();
	}

	return FGSCEditableState::Editable;
}

TSubclassOf<UGSCFormatBase> UGSCData_Setting::GetFormatClass() const
{
	if (Format)
	{
		return TSubclassOf<UGSCFormatBase>(Format->GetClass());
	}

	return nullptr;
}

#undef LOCTEXT_NAMESPACE
