#include "AssetTypeActions_ProceduralLevelGraph.h"

#include "ProceduralLevelGraphEditor/ProceduralLevelGraphEditor.h"
#include "ProceduralLevelGraphRuntime/ProceduralLevelGraphRuntime.h"

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
    return UProceduralLevelGraphRuntime::StaticClass();
}

void FAssetTypeActions_ProceduralLevelGraph::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
    EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

    for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
    {
        if (UProceduralLevelGraphRuntime* Graph = Cast<UProceduralLevelGraphRuntime>(*ObjIt))
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