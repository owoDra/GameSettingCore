// Copyright (C) 2023 owoDra

#include "GSCSubsystemBlueprintLibrary.h"

#include "GSCGameUserSettings.h"
#include "GSCSubsystem.h"

#include "Engine/Engine.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSCSubsystemBlueprintLibrary)

UGSCSubsystem* UGSCSubsystemBlueprintLibrary::GetGSCSubsystem(TSubclassOf<UGSCSubsystem> Class)
{
	auto* GSCGUS{ (GEngine && GEngine->IsInitialized()) ? Cast<UGSCGameUserSettings>(GEngine->GetGameUserSettings()) : nullptr };
	auto* GSCSubsystem{ GSCGUS ? GSCGUS->GetSubsystemBase(Class) : nullptr };
	return GSCSubsystem;
}
