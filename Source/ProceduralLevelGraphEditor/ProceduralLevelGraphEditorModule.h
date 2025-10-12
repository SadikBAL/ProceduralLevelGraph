#pragma once

#include "CoreMinimal.h"
#include "EdGraphUtilities.h"
#include "Modules/ModuleManager.h"
#include "IAssetTools.h"

class IAssetTypeActions;

class FProceduralLevelGraphEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
    TSharedPtr<IAssetTypeActions> ProceduralLevelGraphAssetTypeActions;
    TSharedPtr<FGraphPanelNodeFactory> PLGRoomFactory;
};