#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"

class UProceduralLevelGraphRuntime;
class SGraphEditor;

class FProceduralLevelGraphEditor : public FAssetEditorToolkit, public FNotifyHook
{
public:
    static const FName GraphCanvasTabId;
    static const FName PropertiesTabId;
    void HandleDelete();
    bool CanDelete();
    FProceduralLevelGraphEditor();
    virtual ~FProceduralLevelGraphEditor() override;

    void InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, UProceduralLevelGraphRuntime* InGraph);

    // IToolkit interface
    virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
    virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
    // End of IToolkit interface

    // FAssetEditorToolkit interface
    virtual FName GetToolkitFName() const override;
    virtual FText GetBaseToolkitName() const override;
    virtual FString GetWorldCentricTabPrefix() const override;
    virtual FLinearColor GetWorldCentricTabColorScale() const override;
    virtual void PostInitAssetEditor() override;
    // End of FAssetEditorToolkit interface

private:
    void OnSelectedNodesChanged(const TSet<UObject*>& Objects);
    virtual void NotifyPostChange(const FPropertyChangedEvent& PropertyChangedEvent, FProperty* PropertyThatChanged) override;
    TSharedRef<SDockTab> SpawnTab_GraphCanvas(const FSpawnTabArgs& Args);
    TSharedRef<SDockTab> SpawnTab_Properties(const FSpawnTabArgs& Args);
	 virtual void SaveAsset_Execute() override;
    void SaveGraphToRuntimeData();
    
    void OnGraphChanged(const FEdGraphEditAction& Action);
    
    UProceduralLevelGraphRuntime* GraphAsset;
    TSharedPtr<SGraphEditor> GraphEditorWidget;
    TSharedPtr<class IDetailsView> PropertyWidget;
    TSharedPtr<FUICommandList> CommandList;
    
    TObjectPtr<UTexture2D> GridTexture;
    TSharedPtr<FSlateBrush> GridBrush;
};