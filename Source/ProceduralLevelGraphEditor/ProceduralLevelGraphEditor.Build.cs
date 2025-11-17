using UnrealBuildTool;

public class ProceduralLevelGraphEditor : ModuleRules
{
    public ProceduralLevelGraphEditor(ReadOnlyTargetRules target) : base(target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            [
                "Core",
                "ProceduralLevelGraphRuntime"
            ]
        );

        PrivateDependencyModuleNames.AddRange(
            [
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
                "BlueprintGraph",
                "InputCore"
            ]
        );
    }
}