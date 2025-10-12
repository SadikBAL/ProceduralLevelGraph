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
                "ProceduralLevelGraphRuntime" // Runtime modülümüze bağımlıyız
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "UnrealEd",      // Genel Editör fonksiyonları
                "GraphEditor",   // SGraphEditor widget'ı
                "AssetTools",    // Yeni asset oluşturma menüsü için
                "EditorStyle",
                "KismetWidgets", // Düğüm ve pin widget'ları
                "ToolMenus",     // Menü ve toolbar'lar
                "ApplicationCore",
                "Projects",       // Plugin içeriğini bulmak için
                "GameplayTags",
                "GameplayTagsEditor",
                "BlueprintGraph"
            }
        );
    }
}