// Copyright (C) 2024 owoDra

#include "K2Node_GetGSCSubsystem.h"

#include "GSCSubsystemBlueprintLibrary.h"
#include "GSCSubsystem.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "EdGraph/EdGraphPin.h"
#include "EdGraphSchema_K2.h"
#include "Internationalization/Internationalization.h"
#include "K2Node_CallFunction.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/CompilerResultsLog.h"
#include "KismetCompiler.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(K2Node_GetGSCSubsystem)

class UEdGraph;


void UK2Node_GetGSCSubsystem::AllocateDefaultPins()
{
	// Add blueprint pin

	if (!CustomClass)
	{
		CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Class, USubsystem::StaticClass(), TEXT("Class"));
	}

	// Result pin

	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Object, (CustomClass ? (UClass*)CustomClass : UGSCSubsystem::StaticClass()), UEdGraphSchema_K2::PN_ReturnValue);

	// Skip the UK2Node_GetSubsystem implementation

	UK2Node::AllocateDefaultPins();
}

void UK2Node_GetGSCSubsystem::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	// Skip the UK2Node_GetSubsystem implementation

	UK2Node::ExpandNode(CompilerContext, SourceGraph);

	static const FName Class_ParamName{ TEXT("Class") };

	auto* GetGSCSubsystemNode{ this };
	auto* SpawnClassPin{ GetGSCSubsystemNode->GetClassPin() };
	auto* SpawnNodeResult{ GetGSCSubsystemNode->GetResultPin() };

	auto* SpawnClass{ (SpawnClassPin != nullptr) ? Cast<UClass>(SpawnClassPin->DefaultObject) : nullptr };
	if (SpawnClassPin && (SpawnClassPin->LinkedTo.IsEmpty()) && !SpawnClass)
	{
		CompilerContext.MessageLog.Error(*NSLOCTEXT("K2Node", "GetSubsystem_Error", "Node {0} must have a class specified.").ToString(), GetGSCSubsystemNode);
		GetGSCSubsystemNode->BreakAllNodeLinks();
		return;
	}

	// Choose appropriate underlying Getter

	FName Get_FunctionName;
	if (CustomClass->IsChildOf<UGSCSubsystem>())
	{
		Get_FunctionName = GET_FUNCTION_NAME_CHECKED(UGSCSubsystemBlueprintLibrary, GetGSCSubsystem);
	}
	else
	{
		CompilerContext.MessageLog.Error(*NSLOCTEXT("K2Node", "GetSubsystem_Error", "Node @@ must have a class specified.").ToString(), GetGSCSubsystemNode);
		GetGSCSubsystemNode->BreakAllNodeLinks();
		return;
	}

	auto* CallGetNode{ CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(GetGSCSubsystemNode, SourceGraph) };
	CallGetNode->FunctionReference.SetExternalMember(Get_FunctionName, UGSCSubsystemBlueprintLibrary::StaticClass());
	CallGetNode->AllocateDefaultPins();

	auto* CallCreateClassTypePin{ CallGetNode->FindPinChecked(Class_ParamName) };
	auto* CallCreateResult{ CallGetNode->GetReturnValuePin() };

	if (SpawnClassPin && SpawnClassPin->LinkedTo.Num() > 0)
	{
		// Copy the 'class' connection from the spawn node to 'UGSCSubsystemBlueprintLibrary::GetGSCSubSystem'
		
		CompilerContext.MovePinLinksToIntermediate(*SpawnClassPin, *CallCreateClassTypePin);
	}
	else
	{
		// Copy class literal onto 'UGSCSubsystemBlueprintLibrary::GetGSCSubSystem' call
		
		CallCreateClassTypePin->DefaultObject = *CustomClass;
	}

	// Move result connection from spawn node to 'UGSCSubsystemBlueprintLibrary::Get[something]Subsystem'

	CallCreateResult->PinType = SpawnNodeResult->PinType;
	CompilerContext.MovePinLinksToIntermediate(*SpawnNodeResult, *CallCreateResult);

	// Break any links to the expanded node

	GetGSCSubsystemNode->BreakAllNodeLinks();
}

void UK2Node_GetGSCSubsystem::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	static TArray<UClass*> Subclasses;
	Subclasses.Reset();
	GetDerivedClasses(UGSCSubsystem::StaticClass(), Subclasses);

	auto CustomizeCallback
	{ 
		[](UEdGraphNode* Node, bool bIsTemplateNode, UClass* Subclass)
		{
			auto* TypedNode{ CastChecked<UK2Node_GetGSCSubsystem>(Node) };
			TypedNode->Initialize(Subclass);
		} 
	};

	auto* ActionKey{ GetClass() };
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		for (const auto& Each : Subclasses)
		{
			if (!UEdGraphSchema_K2::IsAllowableBlueprintVariableType(Each, true))
			{
				continue;
			}

			auto* Spawner{ UBlueprintNodeSpawner::Create(ActionKey) };
			check(Spawner);

			Spawner->CustomizeNodeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateStatic(CustomizeCallback, Each);
			ActionRegistrar.AddBlueprintAction(ActionKey, Spawner);
		}
	}
}

FText UK2Node_GetGSCSubsystem::GetTooltipText() const
{
	if (CustomClass)
	{
		return FText::FormatNamed(NSLOCTEXT("K2Node", "GetGSCSubsystem_TooltipFormat", "Get {ClassName} an Game User Settings Subsystem"), TEXT("ClassName"), CustomClass->GetDisplayNameText());
	}

	return NSLOCTEXT("K2Node", "GetGSCSubsystem_InvalidSubsystemTypeTooltip", "Invalid Subsystem Type");
}

FText UK2Node_GetGSCSubsystem::GetMenuCategory() const
{
	return NSLOCTEXT("K2Node", "GetGSCSubsystem_MenuCategory", "Game User Settings Subsystems");
}
