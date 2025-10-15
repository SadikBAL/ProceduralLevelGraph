#pragma once

#include "CoreMinimal.h"
#include "EdGraphUtilities.h"
#include "Modules/ModuleManager.h"
#include "IAssetTools.h"
#include "Room/MazeNodesFactory.h"

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
};