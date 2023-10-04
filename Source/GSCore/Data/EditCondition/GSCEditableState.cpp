// Copyright (C) 2023 owoDra

#include "GSCEditableState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSCEditableState)

const FGSCEditableState FGSCEditableState::Editable		= FGSCEditableState(/*bVisible = */ true, /*bEnabled = */ true);
const FGSCEditableState FGSCEditableState::NotEditable	= FGSCEditableState(/*bVisible = */ true, /*bEnabled = */ false);
const FGSCEditableState FGSCEditableState::Collapsed	= FGSCEditableState(/*bVisible = */ false, /*bEnabled = */ false);


bool FGSCEditableState::operator==(const FGSCEditableState& Other) const
{
	return (bVisible == Other.bVisible) && (bEnabled == Other.bEnabled);
}

void FGSCEditableState::Hide(const FString& DevReason)
{
	bVisible = false;

#if !UE_BUILD_SHIPPING
	HiddenReasons.Add(DevReason);
#endif
}

void FGSCEditableState::Disable(const FText& Reason)
{
	bEnabled = false;
	DisabledReasons.Add(Reason);
}

