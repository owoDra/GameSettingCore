// Copyright (C) 2023 owoDra

using UnrealBuildTool;

public class GSCore : ModuleRules
{
	public GSCore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;	

		PublicIncludePaths.AddRange(
            new string[] 
			{
                ModuleDirectory,
            }
        );			
		

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"InputCore",
				"Engine",
				"Slate",
				"SlateCore",
				"UMG",
				"GameplayTags",
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"ApplicationCore",
				"PropertyPath",
                "DeveloperSettings",
            }
		);
	}
}
