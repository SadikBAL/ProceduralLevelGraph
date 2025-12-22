#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "IAssetTools.h"
#include "Node/Data/MazeNodesFactory.h"

class IAssetTypeActions;

class FProceduralLevelGraphEditorModule : public IModuleInterface
{
public:
    
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    
private:
    TSharedPtr<IAssetTypeActions> ProceduralLevelGraphAssetTypeAction;
    TSharedPtr<IAssetTypeActions> HallDataAssetAssetTypeAction;
    TSharedPtr<FSRoomGraphNodeFactory> SRoomGraphNodeFactory;
    TSharedPtr<FSHallGraphNodeFactory> SHallGraphNodeFactory;
    TSharedPtr<FSRouterGraphNodeFactory> SRouterGraphNodeFactory;
    TSharedPtr<FSEntranceGraphNodeFactory> SEntranceGraphNodeFactory;
    TSharedPtr<FSLayoutGraphNodeFactory> SLayoutGraphNodeFactory;
    
    void OnDeleteActorsBegin();
    void OnObjectPreSave(UObject* Object, FObjectPreSaveContext ObjectPreSaveContext);
    void UpdateMazeLevelInstanceBluprintsDoorReferances(FString LevelInstanceName,TArray<AActor*>& IgnoreList);
};