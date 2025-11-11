#pragma once

#include "CoreMinimal.h"
#include "SGraphPanel.h"
#include "Node/Data/MazeGraphNodeBase.h"
#include "Toolkits/AssetEditorToolkit.h"

class UProceduralLevelGraphRuntime;
class SGraphEditor;
class SGraphPathOverlay : public SCompoundWidget
{
    public:
    SLATE_BEGIN_ARGS(SGraphPathOverlay) {}
    SLATE_ATTRIBUTE(TSharedPtr<SGraphEditor>, GraphEditor)
    
    SLATE_ATTRIBUTE(TArray<UMazeGraphNodeBase*>, PathToDraw)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs)
    {
        GraphEditor = InArgs._GraphEditor;
        PathToDraw = InArgs._PathToDraw;
    }
    virtual int32 OnPaint( const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled ) const override
    {
        SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

        TSharedPtr<SGraphEditor> GraphEditorPtr = GraphEditor.Get();
        if (!GraphEditorPtr.IsValid()) return LayerId;
        SGraphPanel* GraphPanel = GraphEditorPtr->GetGraphPanel();
        if (GraphPanel == nullptr) return LayerId;
        
        const TArray<UMazeGraphNodeBase*>& Path = PathToDraw.Get();
        if (Path.Num() < 2) return LayerId;
        TArray<FVector2f> Points;
        for (int32 i = 0; i < Path.Num() - 1; ++i)
        {
            UMazeGraphNodeBase* NodeA = Path[i];
            UMazeGraphNodeBase* NodeB = Path[i + 1];
            TSharedPtr<SGraphNode> WidgetA = GraphPanel->GetNodeWidgetFromGuid(NodeA->NodeGuid);
            TSharedPtr<SGraphNode> WidgetB = GraphPanel->GetNodeWidgetFromGuid(NodeB->NodeGuid);

            if (WidgetA.IsValid() && WidgetB.IsValid())
            {
                FVector2f PosA = WidgetA->GetPosition2f();
                FVector2f PosB = WidgetB->GetPosition2f();
                Points.Add(PosA);
                Points.Add(PosB);
            }
        }
        FSlateDrawElement::MakeLines(
                   OutDrawElements,
                   LayerId + 1,
                   AllottedGeometry.ToPaintGeometry(),
                   Points,
                   ESlateDrawEffect::None,
                   FLinearColor::Red,
                   true,
                   5.0f
               );
        return LayerId + 1; // Bir sonraki katmana bildir
    }

private:
    TAttribute<TSharedPtr<SGraphEditor>> GraphEditor;
    TAttribute<TArray<UMazeGraphNodeBase*>> PathToDraw;
};
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
    UPROPERTY()
    TArray<UMazeGraphNodeBase*> ShortestPath;

    TArray<UMazeGraphNodeBase*> FindShortestPathToEntrance(UMazeGraphNodeBase* StartNode);
private:
    void OnSelectedNodesChanged(const TSet<UObject*>& Objects);
    virtual void NotifyPostChange(const FPropertyChangedEvent& PropertyChangedEvent, FProperty* PropertyThatChanged) override;
    TSharedRef<SDockTab> SpawnTab_GraphCanvas(const FSpawnTabArgs& Args);
    TSharedRef<SDockTab> SpawnTab_Properties(const FSpawnTabArgs& Args);
	 virtual void SaveAsset_Execute() override;
    bool IsPinVisible(UEdGraphPin* Pin);
    void SaveGraphToRuntimeData();

    
    void OnGraphChanged(const FEdGraphEditAction& Action);
    
    UProceduralLevelGraphRuntime* GraphAsset;
    TSharedPtr<SGraphEditor> GraphEditorWidget;
    TSharedPtr<class IDetailsView> PropertyWidget;
    TSharedPtr<FUICommandList> CommandList;
    UPROPERTY(EditAnywhere)
    FString GraphCanvasTabName;
};