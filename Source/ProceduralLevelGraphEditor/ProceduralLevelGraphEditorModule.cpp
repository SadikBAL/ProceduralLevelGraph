#include "ProceduralLevelGraphEditorModule.h"

#include "EdGraphUtilities.h"
#include "AssetTypeActions/AssetTpyeActions_HallDataAsset.h"
#include "AssetTypeActions/AssetTypeActions_ProceduralLevelGraph.h"

#define LOCTEXT_NAMESPACE "FProceduralLevelGraphEditorModule"

void FProceduralLevelGraphEditorModule::StartupModule()
{
    IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

    const EAssetTypeCategories::Type PLGCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("ProceduralLevelGraph")), LOCTEXT("ProceduralLevelGraphCategory", "Procedural Level Graph"));
    
    ProceduralLevelGraphAssetTypeAction = MakeShareable(new FAssetTypeActions_ProceduralLevelGraph(PLGCategory));
    AssetTools.RegisterAssetTypeActions(ProceduralLevelGraphAssetTypeAction.ToSharedRef());

    HallDataAssetAssetTypeAction = MakeShareable(new FAssetTypeActions_HallDataAsset(PLGCategory));
    AssetTools.RegisterAssetTypeActions(HallDataAssetAssetTypeAction.ToSharedRef());
    
    SRoomGraphNodeFactory = MakeShareable(new FSRoomGraphNodeFactory());
    SHallGraphNodeFactory = MakeShareable(new FSHallGraphNodeFactory());
    SRouterGraphNodeFactory = MakeShareable(new FSRouterGraphNodeFactory());
    SEntranceGraphNodeFactory = MakeShareable(new FSEntranceGraphNodeFactory());
    SLayoutGraphNodeFactory = MakeShareable(new FSLayoutGraphNodeFactory());
    
    FEdGraphUtilities::RegisterVisualNodeFactory(SRoomGraphNodeFactory);
    FEdGraphUtilities::RegisterVisualNodeFactory(SHallGraphNodeFactory);
    FEdGraphUtilities::RegisterVisualNodeFactory(SRouterGraphNodeFactory);
    FEdGraphUtilities::RegisterVisualNodeFactory(SEntranceGraphNodeFactory);
    FEdGraphUtilities::RegisterVisualNodeFactory(SLayoutGraphNodeFactory);
}

void FProceduralLevelGraphEditorModule::ShutdownModule()
{
    if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
    {
        IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
        if (ProceduralLevelGraphAssetTypeAction.IsValid())
        {
            AssetTools.UnregisterAssetTypeActions(ProceduralLevelGraphAssetTypeAction.ToSharedRef());
        }
        if (HallDataAssetAssetTypeAction.IsValid())
        {
            AssetTools.UnregisterAssetTypeActions(HallDataAssetAssetTypeAction.ToSharedRef());
        }
    }
    if (SRoomGraphNodeFactory.IsValid())
    {
        FEdGraphUtilities::UnregisterVisualNodeFactory(SRoomGraphNodeFactory);
    }
    if (SHallGraphNodeFactory.IsValid())
    {
        FEdGraphUtilities::UnregisterVisualNodeFactory(SHallGraphNodeFactory);
    }
    if (SRouterGraphNodeFactory.IsValid())
    {
        FEdGraphUtilities::UnregisterVisualNodeFactory(SRouterGraphNodeFactory);
    }
    if (SEntranceGraphNodeFactory.IsValid())
    {
        FEdGraphUtilities::UnregisterVisualNodeFactory(SEntranceGraphNodeFactory);
    }
    if (SLayoutGraphNodeFactory.IsValid())
    {
        FEdGraphUtilities::UnregisterVisualNodeFactory(SLayoutGraphNodeFactory);
    }
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FProceduralLevelGraphEditorModule, ProceduralLevelGraphEditor)