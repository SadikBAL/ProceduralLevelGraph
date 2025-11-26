#include "ProceduralLevelGraphEditor.h"

#include "EdGraphUtilities.h"
#include "ProceduralLevelGraphRuntime/ProceduralLevelGraphRuntime.h"
#include "EdGraph/EdGraph.h"
#include "Widgets/Docking/SDockTab.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "Maze/MazeGraphSchema.h"
#include "GraphEditAction.h"
#include "EdGraph/EdGraphSchema.h"
#include "Framework/Commands/GenericCommands.h"
#include "GraphEditorActions.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Node/Data/EntranceGraphNode.h"
#include "Node/View/CustomCompoundWidget.h"
#include "ProceduralLevelGraphRuntime/Node/HallNode.h"
#include "ProceduralLevelGraphRuntime/Node/RoomNode.h"
#include "ProceduralLevelGraphRuntime/Node/RouterNode.h"
#include "ProceduralLevelGraphEditor/Node/Data/HallGraphNode.h"
#include "ProceduralLevelGraphEditor/Node/Data/RoomGraphNode.h"
#include "ProceduralLevelGraphEditor/Node/Data/RouterGraphNode.h"
#include "ProceduralLevelGraphRuntime/Node/EntranceRoom.h"
#include "Windows/WindowsPlatformApplicationMisc.h"

#define LOCTEXT_NAMESPACE "ProceduralLevelGraphEditor"

const FName FProceduralLevelGraphEditor::GraphCanvasTabId(TEXT("ProceduralLevelGraphEditor_GraphCanvas"));
const FName FProceduralLevelGraphEditor::PropertiesTabId(TEXT("ProceduralLevelGraphEditor_Properties"));

FDelegateHandle OnGraphChangedDelegateHandle;

void FProceduralLevelGraphEditor::HandleDelete()
{
    const FScopedTransaction Transaction( NSLOCTEXT("UnrealEd", "EditorDeleteSelectedNode", "Delete Selected Node") );

    GraphEditorWidget->GetCurrentGraph()->Modify();

    const FGraphPanelSelectionSet SelectedNodes = GraphEditorWidget->GetSelectedNodes();

    GraphEditorWidget->ClearSelectionSet();

    for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
    {
        UEdGraphNode* Node = CastChecked<UEdGraphNode>(*NodeIt);

        if (Node->CanUserDeleteNode())
        {
            FBlueprintEditorUtils::RemoveNode(NULL, Node, true);
        }
    }
}

bool FProceduralLevelGraphEditor::CanDelete()
{
    return true;
}

FProceduralLevelGraphEditor::FProceduralLevelGraphEditor()
{
    GraphAsset = nullptr;
    CommandList = nullptr;
    
}

FProceduralLevelGraphEditor::~FProceduralLevelGraphEditor()
{
    if (GraphAsset && GraphAsset->EdGraph)
    {
        GraphAsset->EdGraph->RemoveOnGraphChangedHandler(OnGraphChangedDelegateHandle);
    }
}

void FProceduralLevelGraphEditor::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, UProceduralLevelGraphRuntime* InGraph)
{
    GraphAsset = InGraph;

    if (GraphAsset->EdGraph == nullptr)
    {
        GraphAsset->EdGraph = NewObject<UEdGraph>(GraphAsset, FName("MazeGraph_EdGraph"), RF_Transactional);
        GraphAsset->EdGraph->Schema = UMazeGraphSchema::StaticClass();
        GraphAsset->EdGraph->bAllowDeletion = false;
    }
    bool bHasEntranceNode = false;
    for (UEdGraphNode* Node : GraphAsset->EdGraph->Nodes)
    {
        if (Cast<UEntranceGraphNode>(Node))
        {
            bHasEntranceNode = true;
            break;
        }
    }
    
    if (!bHasEntranceNode)
    {
        FGraphNodeCreator<UEntranceGraphNode> NodeCreator(*GraphAsset->EdGraph);
        UEntranceGraphNode* EntranceNode = NodeCreator.CreateNode();
        EntranceNode->NodePosX = 0;
        EntranceNode->NodePosY = 0;
        NodeCreator.Finalize();
    }
    
    OnGraphChangedDelegateHandle = GraphAsset->EdGraph->AddOnGraphChangedHandler(FOnGraphChanged::FDelegate::CreateRaw(this, &FProceduralLevelGraphEditor::OnGraphChanged));
    FGraphEditorCommands::Register();

    TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_MazeGraph_Layout_V1")
        ->AddArea
        (
            FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
            ->Split
            (
                FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)->SetSizeCoefficient(0.9f)
                ->Split
                (
                    FTabManager::NewStack()
                    ->SetSizeCoefficient(0.2f)
                    ->AddTab(PropertiesTabId, ETabState::OpenedTab)
                )
                ->Split
                (
                    FTabManager::NewStack()
                    ->SetSizeCoefficient(0.8f)
                    ->AddTab(GraphCanvasTabId, ETabState::OpenedTab)
                )
            )
        );

    const bool bCreateDefaultStandaloneMenu = true;
    const bool bCreateDefaultToolbar = true;
    CommandList = MakeShareable(new FUICommandList);

    CommandList->MapAction(
        FGenericCommands::Get().Delete,
        FExecuteAction::CreateSP(this, &FProceduralLevelGraphEditor::HandleDelete),
        FCanExecuteAction::CreateSP(this, &FProceduralLevelGraphEditor::CanDelete)
    );

    // Copy
    CommandList->MapAction(
        FGenericCommands::Get().Copy,
        FExecuteAction::CreateSP(this, &FProceduralLevelGraphEditor::CopySelectedNodes)
    );

    // Paste
    CommandList->MapAction(
        FGenericCommands::Get().Paste,
        FExecuteAction::CreateSP(this, &FProceduralLevelGraphEditor::PasteSelectedNodes)
    );

    // Duplicate
    CommandList->MapAction(
        FGenericCommands::Get().Duplicate,
        FExecuteAction::CreateSP(this, &FProceduralLevelGraphEditor::DuplicateSelectedNodes)
    );
    FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost, FName("ProceduralLevelGraphEditorApp"), StandaloneDefaultLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, GraphAsset);
}

void FProceduralLevelGraphEditor::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
    WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_ProceduralLevelGraphEditor", "Procedural Level Graph Editor"));
    auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

    FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

    InTabManager->RegisterTabSpawner(GraphCanvasTabId, FOnSpawnTab::CreateSP(this, &FProceduralLevelGraphEditor::SpawnTab_GraphCanvas))
        .SetDisplayName(LOCTEXT("GraphCanvasTab", "Graph"))
        .SetGroup(WorkspaceMenuCategoryRef)
        .SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "GraphEditor.EventGraph_16x"));

    InTabManager->RegisterTabSpawner(PropertiesTabId, FOnSpawnTab::CreateSP(this, &FProceduralLevelGraphEditor::SpawnTab_Properties))
        .SetDisplayName(LOCTEXT("PropertiesTab", "Details"))
        .SetGroup(WorkspaceMenuCategoryRef)
        .SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details"));
}

void FProceduralLevelGraphEditor::UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
    FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
    InTabManager->UnregisterTabSpawner(GraphCanvasTabId);
    InTabManager->UnregisterTabSpawner(PropertiesTabId);
}

FName FProceduralLevelGraphEditor::GetToolkitFName() const
{
    return FName("ProceduralLevelGraphEditor");
}

FText FProceduralLevelGraphEditor::GetBaseToolkitName() const
{
    return LOCTEXT("AppLabel", "Procedural Level Graph Editor");
}

FString FProceduralLevelGraphEditor::GetWorldCentricTabPrefix() const
{
    return LOCTEXT("WorldCentricTabPrefix", "PLG ").ToString();
}

FLinearColor FProceduralLevelGraphEditor::GetWorldCentricTabColorScale() const
{
    return FLinearColor(0.3f, 0.2f, 0.5f, 0.5f);
}

void FProceduralLevelGraphEditor::PostInitAssetEditor()
{
    FAssetEditorToolkit::PostInitAssetEditor();
    SaveGraphToRuntimeData();
}

TSharedRef<SDockTab> FProceduralLevelGraphEditor::SpawnTab_GraphCanvas(const FSpawnTabArgs& Args)
{
    SGraphEditor::FGraphEditorEvents InEvents;
    InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FProceduralLevelGraphEditor::OnSelectedNodesChanged);
    
    GraphEditorWidget = SNew(SGraphEditor)
        .AdditionalCommands(GetToolkitCommands())
        .GraphToEdit(GraphAsset->EdGraph)
        .GraphEvents(InEvents)
        .AdditionalCommands(CommandList);

    return SNew(SDockTab)
        .Label(LOCTEXT("GraphCanvasTitle", "Graph"))
        [
            SNew(SOverlay)
            + SOverlay::Slot()
            [
                GraphEditorWidget.ToSharedRef()
            ]
            + SOverlay::Slot()
            [
                SNew(SRouteOverlay)
                .GraphEditor(GraphEditorWidget)
                .PathToDraw(TAttribute<FRoutePathMap>::CreateLambda([this]()
                { 
                    return Routes; 
                }))
                .bShowRoutes(TAttribute<bool>::CreateLambda([this]() {
                    return GraphAsset ? GraphAsset->bShowRoutes : false;
                }))
                .Visibility(EVisibility::HitTestInvisible)
            ]
        ];
}

TSharedRef<SDockTab> FProceduralLevelGraphEditor::SpawnTab_Properties(const FSpawnTabArgs& Args)
{
    SGraphEditor::FGraphEditorEvents InEvents;
    InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FProceduralLevelGraphEditor::OnSelectedNodesChanged);
    
    FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
    FDetailsViewArgs DetailsViewArgs;
    DetailsViewArgs.bUpdatesFromSelection = true;
    DetailsViewArgs.bLockable = false;
    DetailsViewArgs.bAllowSearch = true;
    DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::ObjectsUseNameArea;
    DetailsViewArgs.bHideSelectionTip = true;
    DetailsViewArgs.NotifyHook = this;
    
    PropertyWidget = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
    PropertyWidget->SetObject(GraphAsset);

    return SNew(SDockTab)
        .Label(LOCTEXT("PropertiesTitle", "Details"))
        [
            PropertyWidget.ToSharedRef()
        ];
}

void FProceduralLevelGraphEditor::SaveAsset_Execute()
{
    FAssetEditorToolkit::SaveAsset_Execute();
    SaveGraphToRuntimeData();
}

bool FProceduralLevelGraphEditor::IsPinVisible(UEdGraphPin* Pin)
{
    if (!Pin) return false;
    if (UHallGraphNode* HallEdNode = Cast<UHallGraphNode>(Pin->GetOwningNode()))
    {
        FName PinName = Pin->GetFName();
        int32 Rotation = HallEdNode->RoomRotation;
        if (PinName == FName("Up") || PinName == FName("Down"))
        {
            return (Rotation == 0 || Rotation == 180);
        }
        else if (PinName == FName("Left") || PinName == FName("Right"))
        {
            return (Rotation == 90 || Rotation == 270);
        }
    }
    return true;
}

void FProceduralLevelGraphEditor::SaveGraphToRuntimeData()
{
    if (GraphAsset == nullptr || GraphAsset->EdGraph == nullptr) return;
    
    GraphAsset->AllNodes.Empty();
    GraphAsset->StartNode = nullptr;
    
    TMap<UEdGraphNode*, UMazeNodeBase*> NodeMap;
    for (UEdGraphNode* EdNode : GraphAsset->EdGraph->Nodes)
    {
        if (UMazeGraphNodeBase* MazeEdNode = Cast<UMazeGraphNodeBase>(EdNode))
        {
            UMazeNodeBase* NewRuntimeNode = nullptr;
            
            if (URoomGraphNode* RoomEdNode = Cast<URoomGraphNode>(EdNode))
            {
                URoomNode* RuntimeRoom = NewObject<URoomNode>(GraphAsset);
                RuntimeRoom->RoomHeight = RoomEdNode->RoomHeight;
                RuntimeRoom->RoomWidth = RoomEdNode->RoomWidth;
                RuntimeRoom->RoomRotation = RoomEdNode->RoomRotation;
                RuntimeRoom->RoomBlueprints = RoomEdNode->RoomBlueprints;
                RuntimeRoom->HallBlueprints = RoomEdNode->HallBlueprints;
                NewRuntimeNode = RuntimeRoom;
            }
            else if (UHallGraphNode* HallEdNode = Cast<UHallGraphNode>(EdNode))
            {
                UHallNode* RuntimeHall = NewObject<UHallNode>(GraphAsset);
                RuntimeHall->HallLength = HallEdNode->HallLength;
                RuntimeHall->RoomRotation = HallEdNode->RoomRotation;
                RuntimeHall->HallBlueprint = HallEdNode->HallBlueprint;
                NewRuntimeNode = RuntimeHall;
                
            }
            else if (URouterGraphNode* RouterEdNode = Cast<URouterGraphNode>(EdNode))
            {
                URouterNode* RuntimeRouter = NewObject<URouterNode>(GraphAsset);
                RuntimeRouter->RoomRotation = RouterEdNode->RoomRotation;
                NewRuntimeNode = RuntimeRouter;
            }
            else if (UEntranceGraphNode* EntranceEdNode = Cast<UEntranceGraphNode>(EdNode))
            {
                UEntranceNode* RuntimeEntrance = NewObject<UEntranceNode>(GraphAsset);
                RuntimeEntrance->RoomHeight = EntranceEdNode->RoomHeight;
                RuntimeEntrance->RoomWidth = EntranceEdNode->RoomWith;
                RuntimeEntrance->RoomRotation = EntranceEdNode->RoomRotation;
                RuntimeEntrance->RoomPosition = EntranceEdNode->RoomPosition;
                NewRuntimeNode = RuntimeEntrance;
                // Start Location its special node.
                GraphAsset->StartNode = NewRuntimeNode;
            }
            if (NewRuntimeNode)
            {
                //NewRuntimeNode->GameplayTags = MazeEdNode->GameplayTags;
                NewRuntimeNode->NodeGuid = MazeEdNode->NodeGuid;
                NodeMap.Add(EdNode, NewRuntimeNode);
            }
        }
    }
    for (UMazeNodeBase* Node : GraphAsset->AllNodes)
    {
        if (Node)
        {
            Node->UpNode = nullptr;
            Node->DownNode = nullptr;
            Node->LeftNode = nullptr;
            Node->RightNode = nullptr;
            Node->Others.Empty(); 
        }
    }
    for (auto Map : NodeMap)
    {
        UEdGraphNode* EdGraph = Map.Key;
        UMazeNodeBase* RuntimeNode = Map.Value;
        for (UEdGraphPin* LinkedPin : EdGraph->Pins)
        {
            if (LinkedPin->GetName() == FName("Up"))
            {
                for (UEdGraphPin* LinkedTo : LinkedPin->LinkedTo)
                {
                    if (NodeMap.Contains(LinkedTo->GetOwningNode()) && LinkedPin != LinkedTo && IsPinVisible(LinkedPin))
                    {
                        RuntimeNode->UpNode = NodeMap[LinkedTo->GetOwningNode()];
                    }
                }
            }
            else if (LinkedPin->GetName() == FName("Down"))
            {
                for (UEdGraphPin* LinkedTo : LinkedPin->LinkedTo)
                {
                    if (NodeMap.Contains(LinkedTo->GetOwningNode()) && LinkedPin != LinkedTo && IsPinVisible(LinkedPin))
                    {
                        RuntimeNode->DownNode = NodeMap[LinkedTo->GetOwningNode()];
                    }
                }
            }
            else if (LinkedPin->GetName() == FName("Left"))
            {
                for (UEdGraphPin* LinkedTo : LinkedPin->LinkedTo)
                {
                    if (NodeMap.Contains(LinkedTo->GetOwningNode()) && LinkedPin != LinkedTo && IsPinVisible(LinkedPin))
                    {
                        RuntimeNode->LeftNode = NodeMap[LinkedTo->GetOwningNode()];
                    }
                }
            }
            else if (LinkedPin->GetName() == FName("Right"))
            {
                for (UEdGraphPin* LinkedTo : LinkedPin->LinkedTo)
                {
                    if (NodeMap.Contains(LinkedTo->GetOwningNode()) && LinkedPin != LinkedTo && IsPinVisible(LinkedPin))
                    {
                        RuntimeNode->RightNode = NodeMap[LinkedTo->GetOwningNode()];
                    }
                }
            }
        }
        GraphAsset->AllNodes.Add(RuntimeNode);
    }
    if (!GraphAsset->StartNode)
    {
        UE_LOG(LogTemp, Error, TEXT("SaveGraphToRuntimeData Error : StartNode is null. Ist coudnt working on runtime."));
    }
}

void FProceduralLevelGraphEditor::FindAllRoutes(UMazeGraphNodeBase* SelectedNode)
{
    Routes.Empty();
    if (!SelectedNode || !GraphAsset || !GraphAsset->EdGraph)
    {
        return;
    }
    UEntranceGraphNode* TargetNode = nullptr;
    for (UEdGraphNode* Node : GraphAsset->EdGraph->Nodes)
    {
        if (UEntranceGraphNode* EntranceNode = Cast<UEntranceGraphNode>(Node))
        {
            TargetNode = EntranceNode;
            break;
        }
    }
    if (!TargetNode)
    {
        UE_LOG(LogTemp, Warning, TEXT("UpdateAllPathsFromNode: Grafikte Entrance Node bulunamadı!"));
        return;
    }

    if (SelectedNode == TargetNode)
    {
        UE_LOG(LogTemp, Warning, TEXT("UpdateAllPathsFromNode: Grafikte Entrance Node bulunamadı!"));
    }
    else
    {
        TArray<TArray<UMazeGraphNodeBase*>> AllPaths;
        FindAllPaths(SelectedNode, TargetNode, AllPaths);
        AllPaths.Sort([](const TArray<UMazeGraphNodeBase*>& A, const TArray<UMazeGraphNodeBase*>& B)
        {
            return A.Num() < B.Num();
        });
        if (AllPaths.Num() > 0)
        {
            Routes.Add(ERouteType::RouteA, AllPaths[0]);
        }
        if (AllPaths.Num() > 1)
        {
            Routes.Add(ERouteType::RouteB, AllPaths[1]);
        }
        if (AllPaths.Num() > 2)
        {
            Routes.Add(ERouteType::RouteC, AllPaths[2]);
        }
        if (AllPaths.Num() > 3)
        {
            Routes.Add(ERouteType::RouteD, AllPaths[3]);
        }
        UE_LOG(LogTemp, Log, TEXT("Toplam %d adet alternatif yol bulundu."), AllPaths.Num());
    }
}

void FProceduralLevelGraphEditor::FindAllPaths(UMazeGraphNodeBase* StartNode, UMazeGraphNodeBase* TargetNode,
    TArray<TArray<UMazeGraphNodeBase*>>& OutAllPaths)
{
    OutAllPaths.Empty();
    if (!StartNode || !TargetNode)
    {
        return;
    }
    TArray<UMazeGraphNodeBase*> CurrentPath;
    TSet<UMazeGraphNodeBase*> VisitedNodesOnPath;
    FindAllPathsRecursive(StartNode, TargetNode, CurrentPath, VisitedNodesOnPath, OutAllPaths);
}

void FProceduralLevelGraphEditor::FindAllPathsRecursive(UMazeGraphNodeBase* CurrentNode, UMazeGraphNodeBase* TargetNode,
    TArray<UMazeGraphNodeBase*>& CurrentPath, TSet<UMazeGraphNodeBase*>& VisitedNodesOnPath,
    TArray<TArray<UMazeGraphNodeBase*>>& OutAllPaths)
{
    CurrentPath.Add(CurrentNode);
    VisitedNodesOnPath.Add(CurrentNode);
    if (CurrentNode == TargetNode)
    {
        OutAllPaths.Add(CurrentPath);
    }
    else
    {
        TArray<UMazeGraphNodeBase*> Neighbors;
        for (UEdGraphPin* Pin : CurrentNode->Pins)
        {
            if (!IsPinVisible(Pin))
            {
                continue;
            }

            for (UEdGraphPin* OtherPin : Pin->LinkedTo)
            {
                if (UMazeGraphNodeBase* NeighborNode = Cast<UMazeGraphNodeBase>(OtherPin->GetOwningNode()))
                {
                    Neighbors.AddUnique(NeighborNode);
                }
            }
        }
        for (UMazeGraphNodeBase* Neighbor : Neighbors)
        {
            if (Neighbor && !VisitedNodesOnPath.Contains(Neighbor))
            {
                FindAllPathsRecursive(Neighbor, TargetNode, CurrentPath, VisitedNodesOnPath, OutAllPaths);
            }
        }
    }
    VisitedNodesOnPath.Remove(CurrentNode);
    CurrentPath.RemoveAt(CurrentPath.Num() - 1);
}

void FProceduralLevelGraphEditor::OnGraphChanged(const FEdGraphEditAction& Action)
{
    if (Action.Action == GRAPHACTION_AddNode ||
        Action.Action == GRAPHACTION_RemoveNode
        )
    {
        GraphAsset->MarkPackageDirty();
    }
    SaveGraphToRuntimeData();
}

void FProceduralLevelGraphEditor::CopySelectedNodes()
{
    const FGraphPanelSelectionSet SelectedNodes = GraphEditorWidget->GetSelectedNodes();
    FGraphPanelSelectionSet NodesToCopy;
    for (UObject* Obj : SelectedNodes)
    {
        if (UMazeGraphNodeBase* Node = Cast<UMazeGraphNodeBase>(Obj))
        {
            if (Node->CanUserCopyNode())
            {
                NodesToCopy.Add(Node);
            }
        }
    }
    if (NodesToCopy.Num() > 0)
    {
        FString ExportedText;
        FEdGraphUtilities::ExportNodesToText(NodesToCopy, ExportedText);
        FPlatformApplicationMisc::ClipboardCopy(*ExportedText);
    }
}

void FProceduralLevelGraphEditor::PasteSelectedNodes()
{
    if (GraphEditorWidget.IsValid())
    {
        FString TextToImport;
        FPlatformApplicationMisc::ClipboardPaste(TextToImport);
        const FScopedTransaction Transaction(NSLOCTEXT("ProceduralLevelGraphEditor", "PasteNode", "Paste Node"));
        
        GraphAsset->EdGraph->Modify();
        const FVector2f PasteLocation = GraphEditorWidget->GetPasteLocation2f();
        TSet<UEdGraphNode*> PastedNodes;
        FEdGraphUtilities::ImportNodesFromText(GraphAsset->EdGraph, TextToImport, PastedNodes);

        FVector2D AvgNodePosition(0.0f, 0.0f);
        for (UEdGraphNode* Node : PastedNodes)
        {
            AvgNodePosition.X += Node->NodePosX;
            AvgNodePosition.Y += Node->NodePosY;
        }

        if (PastedNodes.Num() > 0)
        {
            const float InvNumNodes = 1.0f / float(PastedNodes.Num());
            AvgNodePosition.X *= InvNumNodes;
            AvgNodePosition.Y *= InvNumNodes;
        }

        for (UEdGraphNode* Node : PastedNodes)
        {
            Node->CreateNewGuid();
            Node->NodePosX = (Node->NodePosX - AvgNodePosition.X) + PasteLocation.X;
            Node->NodePosY = (Node->NodePosY - AvgNodePosition.Y) + PasteLocation.Y;
            Node->SnapToGrid(50);
            Node->Pins.Empty();
            Node->AllocateDefaultPins();
            Node->ReconstructNode();
        }
        GraphEditorWidget->ClearSelectionSet();
        for (UEdGraphNode* Node : PastedNodes)
        {
            GraphEditorWidget->SetNodeSelection(Node, true);
        }
        GraphEditorWidget->NotifyGraphChanged();
        SaveGraphToRuntimeData();
    }
}

void FProceduralLevelGraphEditor::DuplicateSelectedNodes()
{
    CopySelectedNodes();
    PasteSelectedNodes();
}

void FProceduralLevelGraphEditor::OnSelectedNodesChanged(const TSet<class UObject*>& NewSelection)
{
    Routes.Empty();
    if (PropertyWidget.IsValid())
    {
        if (NewSelection.Num() > 0)
        {
            if (NewSelection.Num() == 1 && GraphAsset->bShowRoutes)
            {
                if (UMazeGraphNodeBase* SelectedNode = Cast<UMazeGraphNodeBase>(NewSelection.Array()[0]))
                {
                    FindAllRoutes(SelectedNode);
                }
            }
            PropertyWidget->SetObjects(NewSelection.Array());
        }
        else
        {
            TArray<UObject*> AssetArray;
            AssetArray.Add(GraphAsset);
            PropertyWidget->SetObjects(AssetArray);
        }
    }
}

void FProceduralLevelGraphEditor::NotifyPostChange(const FPropertyChangedEvent& PropertyChangedEvent, FProperty* PropertyThatChanged)
{
	if(GraphEditorWidget.IsValid())
	{
		GraphEditorWidget->NotifyGraphChanged();
	}
}

#undef LOCTEXT_NAMESPACE