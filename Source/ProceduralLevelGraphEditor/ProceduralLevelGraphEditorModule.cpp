#include "ProceduralLevelGraphEditorModule.h"

#include "EdGraphUtilities.h"
#include "EngineUtils.h"
#include "ProceduralLevelGraphEditor.h"
#include "Selection.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetTypeActions/AssetTpyeActions_HallDataAsset.h"
#include "AssetTypeActions/AssetTypeActions_ProceduralLevelGraph.h"
#include "Node/Data/RoomGraphNode.h"
#include "ProceduralLevelGraphRuntime/ProceduralLevelGraphRuntime.h"
#include "ProceduralLevelGraphRuntime/LevelInstance/MazeTileLevelInstance.h"
#include "ProceduralLevelGraphRuntime/LevelInstance/PassagePoint.h"
#include "UObject/ObjectSaveContext.h"

#define LOCTEXT_NAMESPACE "FProceduralLevelGraphEditorModule"

void FProceduralLevelGraphEditorModule::OnObjectPreSave(UObject* Object, FObjectPreSaveContext ObjectPreSaveContext)
{
    AActor* SavedActor = Cast<AActor>(Object);
    UWorld* WorldToScan = nullptr;
    if (SavedActor)
    {
        WorldToScan = SavedActor->GetWorld();
    }
    if (WorldToScan &&    WorldToScan->GetMapName().Contains("LI_") && (WorldToScan->WorldType == EWorldType::Editor || WorldToScan->WorldType == EWorldType::EditorPreview))
    {
        UE_LOG(LogTemp, Log, TEXT("Updated Map Name : %s"), *WorldToScan->GetName());
        TArray<AActor*> IgnoreList;
        UpdateMazeLevelInstanceBlueprintsDoorRef(WorldToScan->GetName(),IgnoreList);
        
    }
}

void FProceduralLevelGraphEditorModule::UpdateMazeLevelInstanceBlueprintsDoorRef(FString LevelInstanceName, TArray<AActor*>& IgnoreList)
{
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    FARFilter Filter;
    Filter.ClassPaths.Add(UBlueprint::StaticClass()->GetClassPathName());
    Filter.bRecursiveClasses = true;

    TArray<FAssetData> AssetList;
    AssetRegistryModule.Get().GetAssets(Filter, AssetList);
    UClass* TargetBaseClass = AMazeTileLevelInstance::StaticClass();

    for (const FAssetData& AssetData : AssetList)
    {
        UBlueprint* BlueprintAsset = Cast<UBlueprint>(AssetData.GetAsset());
        if (BlueprintAsset && BlueprintAsset->GeneratedClass)
        {
            if (BlueprintAsset->GeneratedClass->IsChildOf(TargetBaseClass))
            {
               
                if ( const UClass* BPClass = BlueprintAsset->GeneratedClass)
                {
                    if (AMazeTileLevelInstance* DefaultObject = Cast<AMazeTileLevelInstance>(BPClass->GetDefaultObject()))
                    {
                        if (DefaultObject->GetWorldAsset() && DefaultObject->GetWorldAsset()->GetName() == LevelInstanceName)
                        {
                            UE_LOG(LogTemp, Warning, TEXT("Blueprint: %s"), *AssetData.AssetName.ToString());
                            DefaultObject->LoadMapData(IgnoreList);
                        }
                    }
                }
            }
        }
    }
}

void FProceduralLevelGraphEditorModule::StartupModule()
{
    IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    
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
    

    FEditorDelegates::OnDeleteActorsBegin.AddRaw(this, &FProceduralLevelGraphEditorModule::OnDeleteActorsBegin);
    FCoreUObjectDelegates::OnObjectPreSave.AddRaw(this, &FProceduralLevelGraphEditorModule::OnObjectPreSave);
    
}
void FProceduralLevelGraphEditorModule::OnDeleteActorsBegin()
{
    if (!GEditor) return;

    USelection* SelectedActors = GEditor->GetSelectedActors();
    if (SelectedActors)
    {
        for (FSelectionIterator It(*SelectedActors); It; ++It)
        {
            APassagePoint* Actor = Cast<APassagePoint>(*It);
            if (Actor)
            {
                TArray<AActor*> IgnoreList;
                IgnoreList.Add(Actor);
                UpdateMazeLevelInstanceBlueprintsDoorRef(Actor->GetWorld()->GetName(),IgnoreList);
            }
        }
    }
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
    FCoreUObjectDelegates::OnObjectPreSave.RemoveAll(this);
    
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FProceduralLevelGraphEditorModule, ProceduralLevelGraphEditor)