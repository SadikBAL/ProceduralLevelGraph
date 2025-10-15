#include "AssetTypeActions_ProceduralLevelGraph.h"

#include "ProceduralLevelGraphEditor/ProceduralLevelGraphEditor.h"
#include "ProceduralLevelGraphRuntime/MazeGraph.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions_ProceduralLevelGraph"

FAssetTypeActions_ProceduralLevelGraph::FAssetTypeActions_ProceduralLevelGraph(EAssetTypeCategories::Type InAssetCategory)
    : MyAssetCategory(InAssetCategory)
{
}

FText FAssetTypeActions_ProceduralLevelGraph::GetName() const
{
    return LOCTEXT("AssetTypeName", "Procedural Level Graph");
}

FColor FAssetTypeActions_ProceduralLevelGraph::GetTypeColor() const
{
    return FColor(129, 38, 192);
}

UClass* FAssetTypeActions_ProceduralLevelGraph::GetSupportedClass() const
{
    return UMazeGraph::StaticClass();
}

void FAssetTypeActions_ProceduralLevelGraph::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
    EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

    for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
    {
        if (UMazeGraph* Graph = Cast<UMazeGraph>(*ObjIt))
        {
            TSharedRef<FProceduralLevelGraphEditor> NewEditor(new FProceduralLevelGraphEditor());
            NewEditor->InitEditor(Mode, EditWithinLevelEditor, Graph);
        }
    }
}

uint32 FAssetTypeActions_ProceduralLevelGraph::GetCategories()
{
    return MyAssetCategory;
}

#undef LOCTEXT_NAMESPACE