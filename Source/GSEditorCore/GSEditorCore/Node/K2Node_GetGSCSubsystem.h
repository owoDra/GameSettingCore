// Copyright (C) 2023 owoDra

#pragma once

#include "K2Node_GetSubsystem.h"

#include "K2Node_GetGSCSubsystem.generated.h"

class FBlueprintActionDatabaseRegistrar;
class FKismetCompilerContext;
class UEdGraph;


/**
 * Class that implements a node to access GSCSubsystem added from a blueprint
 */
UCLASS()
class UK2Node_GetGSCSubsystem : public UK2Node_GetSubsystem
{
	GENERATED_BODY()
public:

	virtual void AllocateDefaultPins() override;
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;

	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual FText GetTooltipText() const override;
	virtual FText GetMenuCategory() const override;
	
};
