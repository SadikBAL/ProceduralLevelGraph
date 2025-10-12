#include "ProceduralLevelGraphEditorModule.h"
#include "AssetTypeActions_ProceduralLevelGraph.h"
#include "EdGraphUtilities.h"
#include "PLGRoomFactory.h"

#define LOCTEXT_NAMESPACE "FProceduralLevelGraphEditorModule"

void FProceduralLevelGraphEditorModule::StartupModule()
{
    IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

    // Custom asset kategorisi oluştur
    EAssetTypeCategories::Type PLGCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("ProceduralLevelGraph")), LOCTEXT("ProceduralLevelGraphCategory", "Procedural Level Graph"));

    // Asset'imizi kaydet
    ProceduralLevelGraphAssetTypeActions = MakeShareable(new FAssetTypeActions_ProceduralLevelGraph(PLGCategory));
    AssetTools.RegisterAssetTypeActions(ProceduralLevelGraphAssetTypeActions.ToSharedRef());

    // 1. Yeni fabrikamızdan bir obje oluşturuyoruz.
    PLGRoomFactory = MakeShareable(new FPLGRoomFactory());
    
    // 2. Oluşturduğumuz fabrikayı Unreal Engine'in görsel nod fabrikaları listesine kaydediyoruz.
    FEdGraphUtilities::RegisterVisualNodeFactory(PLGRoomFactory);
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
    // Modül kapanırken, fabrikamızın kaydını temizliyoruz.
    if (PLGRoomFactory.IsValid())
    {
        FEdGraphUtilities::UnregisterVisualNodeFactory(PLGRoomFactory);
        PLGRoomFactory.Reset();
    }
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FProceduralLevelGraphEditorModule, ProceduralLevelGraphEditor)