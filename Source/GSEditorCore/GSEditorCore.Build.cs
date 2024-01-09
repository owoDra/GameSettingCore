// Copyright (C) 2024 owoDra

using UnrealBuildTool;

public class GSEditorCore : ModuleRules
{
	public GSEditorCore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;	

		PublicIncludePaths.AddRange(
            new string[] 
			{
                ModuleDirectory,
                ModuleDirectory + "/GSEditorCore",
            }
        );			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
                "UnrealEd",
                "Slate",
				"SlateCore",
                "BlueprintGraph",
                "KismetCompiler",
                "UMG",
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"ApplicationCore",
				"PropertyPath",
                "DeveloperSettings",
                "ToolWidgets",
                "ClassViewer",
                "AssetTools",
                "GSCore",
            }
		);
	}
}
