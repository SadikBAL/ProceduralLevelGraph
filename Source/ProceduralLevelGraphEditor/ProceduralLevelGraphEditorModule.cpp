#include "ProceduralLevelGraphEditorModule.h"
#include "AssetTypeActions_ProceduralLevelGraph.h"
#include "EdGraphUtilities.h"

#define LOCTEXT_NAMESPACE "FProceduralLevelGraphEditorModule"

void FProceduralLevelGraphEditorModule::StartupModule()
{
    IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

    // Custom asset kategorisi oluştur
    EAssetTypeCategories::Type PLGCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("ProceduralLevelGraph")), LOCTEXT("ProceduralLevelGraphCategory", "Procedural Level Graph"));

    // Asset'imizi kaydet
    ProceduralLevelGraphAssetTypeActions = MakeShareable(new FAssetTypeActions_ProceduralLevelGraph(PLGCategory));
    AssetTools.RegisterAssetTypeActions(ProceduralLevelGraphAssetTypeActions.ToSharedRef());
}

void FProceduralLevelGraphEditorModule::ShutdownModule()
{
    if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
    {
        IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
        if (ProceduralLevelGraphAssetTypeActions.IsValid())
        {
            AssetTools.UnregisterAssetTypeActions(ProceduralLevelGraphAssetTypeActions.ToSharedRef());
        }
    }
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FProceduralLevelGraphEditorModule, ProceduralLevelGraphEditor)