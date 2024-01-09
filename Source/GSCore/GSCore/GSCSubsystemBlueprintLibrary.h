// Copyright (C) 2024 owoDra

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "GSCSubsystemBlueprintLibrary.generated.h"

class UGSCSubsystem;

UCLASS(MinimalAPI)
class UGSCSubsystemBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	/** 
	 * Get a GSC Subsystem from the Game User Settings associated with the provided context 
	 */
	UFUNCTION(BlueprintPure, Category = "Game User Settings Subsystems", meta = (BlueprintInternalUseOnly = "false"))
	static GSCORE_API UGSCSubsystem* GetGSCSubsystem(TSubclassOf<UGSCSubsystem> Class);

};
