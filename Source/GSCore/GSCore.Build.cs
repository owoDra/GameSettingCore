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
                ModuleDirectory + "/GSCore",
            }
        );			
		

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"UMG",
                "InputCore",
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
