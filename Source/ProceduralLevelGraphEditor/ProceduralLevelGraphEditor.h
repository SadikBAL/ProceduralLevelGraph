#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"

class UProceduralLevelGraph;
class SGraphEditor;

class FProceduralLevelGraphEditor : public FAssetEditorToolkit, public FNotifyHook
{
public:
    static const FName GraphCanvasTabId;
    static const FName PropertiesTabId;
    void HandleDelete();
    bool CanDelete();
    FProceduralLevelGraphEditor();
    virtual ~FProceduralLevelGraphEditor();

    void InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, UProceduralLevelGraph* InGraph);

    // IToolkit interface
    virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
    virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
    // End of IToolkit interface

    // FAssetEditorToolkit interface
    virtual FName GetToolkitFName() const override;
    virtual FText GetBaseToolkitName() const override;
    virtual FString GetWorldCentricTabPrefix() const override;
    virtual FLinearColor GetWorldCentricTabColorScale() const override;
    // End of FAssetEditorToolkit interface

private:
    void OnSelectedNodesChanged(const TSet<UObject*>& Objects);
    void NotifyPostChange(const FPropertyChangedEvent& PropertyChangedEvent, FProperty* PropertyThatChanged);
    TSharedRef<SDockTab> SpawnTab_GraphCanvas(const FSpawnTabArgs& Args);
    TSharedRef<SDockTab> SpawnTab_Properties(const FSpawnTabArgs& Args);

    void OnGraphChanged(const FEdGraphEditAction& Action);
    
    UProceduralLevelGraph* GraphAsset;
    TSharedPtr<SGraphEditor> GraphEditorWidget;
    TSharedPtr<class IDetailsView> PropertyWidget;
    TSharedPtr<FUICommandList> CommandList;
};