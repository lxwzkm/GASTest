// Copyright fpwong. All Rights Reserved.

using UnrealBuildTool;

public class BlueprintAssist : ModuleRules
{
	public BlueprintAssist(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.NoPCHs;
		bUseUnity = false;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...
				"GraphEditor",
				"Kismet",
				"KismetWidgets",
				"InputCore",
				"BlueprintGraph",
				"AssetTools",
				"EditorStyle",
				"EditorWidgets",
				"UnrealEd",
				"Projects",
				"Json",
				"JsonUtilities",
				"EngineSettings",
				"AssetRegistry",
				"Persona",
				"WorkspaceMenuStructure",
				"ToolMenus",
				"UMG",
				"RenderCore",
				"DeveloperSettings",
				"CoreUObject",
				"Blutility", 
				"UMGEditor",
				"PropertyEditor",
				"ApplicationCore"
			}
		);

		if (Target.Type == TargetType.Editor)
		{
			PrivateDependencyModuleNames.Add("MessageLog");
		}

		if (Target.bWithLiveCoding)
		{
			PrivateIncludePathModuleNames.Add("LiveCoding");
		}

		if (Target.Version.MajorVersion == 5)
		{
			PrivateDependencyModuleNames.Add("ContentBrowserData");
			PrivateDependencyModuleNames.Add("SubobjectEditor");
			PrivateDependencyModuleNames.Add("SubobjectDataInterface");
			PrivateDependencyModuleNames.Add("EditorFramework");
		}

		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
		);
	}
}