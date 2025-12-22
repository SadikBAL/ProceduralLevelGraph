#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "IAssetTools.h"
#include "Node/Data/MazeNodesFactory.h"

class IAssetTypeActions;

class FProceduralLevelGraphEditorModule : public IModuleInterface
{
public:
    void OnMapSaved(UWorld* World, FObjectPostSaveContext ObjectPostSaveContext);
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
    
};