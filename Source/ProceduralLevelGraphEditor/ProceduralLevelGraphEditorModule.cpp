#include "ProceduralLevelGraphEditorModule.h"

#include "EdGraphUtilities.h"
#include "AssetTypeActions/AssetTypeActions_ProceduralLevelGraph.h"

#define LOCTEXT_NAMESPACE "FProceduralLevelGraphEditorModule"

void FProceduralLevelGraphEditorModule::StartupModule()
{
    IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
    
    EAssetTypeCategories::Type PLGCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("ProceduralLevelGraph")), LOCTEXT("ProceduralLevelGraphCategory", "Procedural Level Graph"));
    
    AssetTypeActions = MakeShareable(new FAssetTypeActions_ProceduralLevelGraph(PLGCategory));
    AssetTools.RegisterAssetTypeActions(AssetTypeActions.ToSharedRef());

    SRoomGraphNodeFactory = MakeShareable(new FSRoomGraphNodeFactory());
    SHallGraphNodeFactory = MakeShareable(new FSHallGraphNodeFactory());
    SRouterGraphNodeFactory = MakeShareable(new FSRouterGraphNodeFactory());
    SEntranceGraphNodeFactory = MakeShareable(new FSEntranceGraphNodeFactory());
    SFloorGraphNodeFactory = MakeShareable(new FSFloorGraphNodeFactory());
    
    FEdGraphUtilities::RegisterVisualNodeFactory(SRoomGraphNodeFactory);
    FEdGraphUtilities::RegisterVisualNodeFactory(SHallGraphNodeFactory);
    FEdGraphUtilities::RegisterVisualNodeFactory(SRouterGraphNodeFactory);
    FEdGraphUtilities::RegisterVisualNodeFactory(SEntranceGraphNodeFactory);
    FEdGraphUtilities::RegisterVisualNodeFactory(SFloorGraphNodeFactory);
}

void FProceduralLevelGraphEditorModule::ShutdownModule()
{
    if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
    {
        IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
        if (AssetTypeActions.IsValid())
        {
            AssetTools.UnregisterAssetTypeActions(AssetTypeActions.ToSharedRef());
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
    if (SFloorGraphNodeFactory.IsValid())
    {
        FEdGraphUtilities::UnregisterVisualNodeFactory(SFloorGraphNodeFactory);
    }
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FProceduralLevelGraphEditorModule, ProceduralLevelGraphEditor)