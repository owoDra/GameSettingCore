// Copyright (C) 2023 owoDra

#include "GSCData_Setting.h"

#include "Definition/Format/GSCFormatBase.h"
#include "Definition/EditCondition/GSCEditConditionBase.h"

#if WITH_EDITOR

#include "Misc/DataValidation.h"

#endif // WITH_EDITOR

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSCData_Setting)


#define LOCTEXT_NAMESPACE "SettingData"

UGSCData_Setting::UGSCData_Setting(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

#if WITH_EDITOR

EDataValidationResult UGSCData_Setting::IsDataValid(FDataValidationContext& Context)
{
	auto Result{ CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid) };
	auto DataName{ FText::FromString(GetNameSafe(this)) };

	if (!Format)
	{
		Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);
		Context.AddError(FText::Format(
			LOCTEXT("InvalidFormat", "No Format selected in [{0}]"), DataName));
	}
	else
	{
		Result = CombineDataValidationResults(Result, Format->IsDataValid(Context));
	}
	
	return Result;
}

#endif // WITH_EDITOR


void UGSCData_Setting::RequestRefreshSetting()
{
	check(Format);
	Format->HandleRefreshSettingRequest();

	if (EditCondition)
	{
		EditCondition->HandleRefreshSettingRequest();
	}
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

#if WITH_EDITOR

void UGSCData_Setting::SetFormatClass(TSubclassOf<UGSCFormatBase> InClass)
{
	if (InClass)
	{
		Format = NewObject<UGSCFormatBase>(this, InClass);
	}
}

#endif // WITH_EDITOR

#undef LOCTEXT_NAMESPACE
