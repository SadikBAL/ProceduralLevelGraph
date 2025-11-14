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
    TSharedPtr<IAssetTypeActions> AssetTypeActions;
    TSharedPtr<FSRoomGraphNodeFactory> SRoomGraphNodeFactory;
    TSharedPtr<FSHallGraphNodeFactory> SHallGraphNodeFactory;
    TSharedPtr<FSRouterGraphNodeFactory> SRouterGraphNodeFactory;
    TSharedPtr<FSEntranceGraphNodeFactory> SEntranceGraphNodeFactory;
};