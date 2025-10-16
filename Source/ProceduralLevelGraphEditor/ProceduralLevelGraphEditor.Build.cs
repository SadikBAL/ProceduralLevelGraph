using UnrealBuildTool;

public class ProceduralLevelGraphEditor : ModuleRules
{
    public ProceduralLevelGraphEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "ProceduralLevelGraphRuntime"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "UnrealEd",
                "GraphEditor",
                "AssetTools",
                "EditorStyle",
                "KismetWidgets",
                "ToolMenus",
                "ApplicationCore",
                "Projects",
                "GameplayTags",
                "GameplayTagsEditor",
                "BlueprintGraph"
            }
        );
    }
}