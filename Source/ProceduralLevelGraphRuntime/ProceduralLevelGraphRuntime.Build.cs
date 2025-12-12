using UnrealBuildTool;

public class ProceduralLevelGraphRuntime : ModuleRules
{
    public ProceduralLevelGraphRuntime(ReadOnlyTargetRules target) : base(target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            [
                "Core",
                "NavigationSystem"
            ]
        );

        PrivateDependencyModuleNames.AddRange(
            [
                "CoreUObject",
                "Engine",
                "GameplayTags"
            ]
        );
        
        if (Target.bBuildEditor)
        {
            PrivateDependencyModuleNames.AddRange(
                [
                    "UnrealEd",
                    "LevelEditor"
                ]
            );
        }
    }
}