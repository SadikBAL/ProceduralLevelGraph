#include "ProceduralLevelGraphEditor.h"

#include "EdGraphUtilities.h"
#include "EngineUtils.h"
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
#include "Node/Data/LayoutGraphNode.h"
#include "Node/View/CustomCompoundWidget.h"
#include "ProceduralLevelGraphRuntime/Node/HallNode.h"
#include "ProceduralLevelGraphRuntime/Node/RoomNode.h"
#include "ProceduralLevelGraphRuntime/Node/RouterNode.h"
#include "ProceduralLevelGraphEditor/Node/Data/HallGraphNode.h"
#include "ProceduralLevelGraphEditor/Node/Data/RoomGraphNode.h"
#include "ProceduralLevelGraphEditor/Node/Data/RouterGraphNode.h"
#include "ProceduralLevelGraphRuntime/LevelInstance/PassagePoint.h"
#include "ProceduralLevelGraphRuntime/Node/EntranceRoom.h"
#include "UObject/ObjectSaveContext.h"
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
        if (UEdGraphNode* NodeRef = CastChecked<UEdGraphNode>(*NodeIt); NodeRef->CanUserDeleteNode())
        {
            FBlueprintEditorUtils::RemoveNode(nullptr, NodeRef, true);
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
    if (GEditor)
    {
        GEditor->UnregisterForUndo(this);
    }
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

    const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_MazeGraph_Layout_V1")
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

    constexpr bool bCreateDefaultStandaloneMenu = true;
    constexpr bool bCreateDefaultToolbar = true;
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

    if (GEditor)
    {
        GEditor->RegisterForUndo(this);
    }
    
    FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost, FName("ProceduralLevelGraphEditorApp"), StandaloneDefaultLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, GraphAsset);
}

void FProceduralLevelGraphEditor::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
    WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_ProceduralLevelGraphEditor", "Procedural Level Graph Editor"));
    const auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

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

void FProceduralLevelGraphEditor::PostUndo(bool bSuccess)
{
    if (bSuccess)
    {
        if (GraphEditorWidget.IsValid())
        {
            GraphEditorWidget->ClearSelectionSet();
            GraphEditorWidget->NotifyGraphChanged();
        }
        SaveGraphToRuntimeData();
        FSlateApplication::Get().DismissAllMenus();
    }
}

void FProceduralLevelGraphEditor::PostRedo(bool bSuccess)
{
    if (bSuccess)
    {
        if (GraphEditorWidget.IsValid())
        {
            GraphEditorWidget->ClearSelectionSet();
            GraphEditorWidget->NotifyGraphChanged();
        }
        SaveGraphToRuntimeData();
        FSlateApplication::Get().DismissAllMenus();
    }
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
            
            if (UEntranceGraphNode* EntranceEdNode = Cast<UEntranceGraphNode>(EdNode))
            {
                UEntranceNode* RuntimeEntrance = NewObject<UEntranceNode>(GraphAsset);
                RuntimeEntrance->RoomHeight = EntranceEdNode->RoomHeight;
                RuntimeEntrance->RoomWidth = EntranceEdNode->RoomWidth;
                RuntimeEntrance->RoomPosition = EntranceEdNode->RoomPosition;
                RuntimeEntrance->RoomLevelInstanceRefs = EntranceEdNode->RoomLevelInstanceRef;
                RuntimeEntrance->HallLevelInstanceRefs = EntranceEdNode->HallLevelInstanceRef;
                NewRuntimeNode = RuntimeEntrance;
                // Start Location its special node.
                GraphAsset->StartNode = NewRuntimeNode;
            }
            else if (URoomGraphNode* RoomEdNode = Cast<URoomGraphNode>(EdNode))
            {
                URoomNode* RuntimeRoom = NewObject<URoomNode>(GraphAsset);
                RuntimeRoom->RoomHeight = RoomEdNode->RoomHeight;
                RuntimeRoom->RoomWidth = RoomEdNode->RoomWidth;
                RuntimeRoom->RoomLevelInstanceRefs = RoomEdNode->RoomLevelInstanceRef;
                RuntimeRoom->HallLevelInstanceRefs = RoomEdNode->HallLevelInstanceRef;
                NewRuntimeNode = RuntimeRoom;
            }
            else if (UHallGraphNode* HallEdNode = Cast<UHallGraphNode>(EdNode))
            {
                UHallNode* RuntimeHall = NewObject<UHallNode>(GraphAsset);
                RuntimeHall->HallLength = HallEdNode->HallLength;
                RuntimeHall->RoomRotation = HallEdNode->RoomRotation;
                RuntimeHall->HallData = HallEdNode->HallData;
                NewRuntimeNode = RuntimeHall;
                
            }
            else if (URouterGraphNode* RouterEdNode = Cast<URouterGraphNode>(EdNode))
            {
                URouterNode* RuntimeRouter = NewObject<URouterNode>(GraphAsset);
                RuntimeRouter->RoomHeight = RouterEdNode->RoomHeight;
                RuntimeRouter->RoomWidth = RouterEdNode->RoomWidth;
                RuntimeRouter->RoomLevelInstanceRefs = RouterEdNode->RoomLevelInstanceRef;
                RuntimeRouter->HallLevelInstanceRefs = RouterEdNode->HallLevelInstanceRef;
                NewRuntimeNode = RuntimeRouter;
            }
            
            if (NewRuntimeNode)
            {
                NewRuntimeNode->RoomRotation = MazeEdNode->RoomRotation;
                NewRuntimeNode->RoomFloor = MazeEdNode->RoomFloor;
                NewRuntimeNode->DoorData = MazeEdNode->DoorData;
                NewRuntimeNode->NodeGuid = MazeEdNode->NodeGuid;
                NodeMap.Add(EdNode, NewRuntimeNode);
            }
        }
    }
    for (const auto Map : NodeMap)
    {
        UEdGraphNode* EdGraph = Map.Key;
        UMazeNodeBase* RuntimeNode = Map.Value;
        for (int i = 0; i < EdGraph->Pins.Num(); i++)
        {
            if (!RuntimeNode->DoorData.IsEmpty() && i < RuntimeNode->DoorData.Num())
            {
                RuntimeNode->DoorData[i].LinkedNode = nullptr;
                if (UEdGraphPin* LinkedPin = EdGraph->Pins[i])
                {
                    if (LinkedPin->LinkedTo.Num() > 0)
                    {
                        if (UEdGraphPin* LinkedTo = LinkedPin->LinkedTo[0])
                        {
                            if (NodeMap.Contains(LinkedTo->GetOwningNode()) && LinkedPin != LinkedTo && IsPinVisible(LinkedPin))
                            {
                                RuntimeNode->DoorData[i].LinkedNode = NodeMap[LinkedTo->GetOwningNode()];
                            }
                        }
                    }
                }
            }
        }
        GraphAsset->AllNodes.Add(RuntimeNode);
    }
    if (!GraphAsset->StartNode)
    {
        UE_LOG(LogTemp, Error, TEXT("SaveGraphToRuntimeData Error : StartNode is null. Ist couldn't working on runtime."));
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
        UE_LOG(LogTemp, Warning, TEXT("UpdateAllPathsFromNode: Entrance node not fount."));
        return;
    }

    if (SelectedNode == TargetNode)
    {
        UE_LOG(LogTemp, Warning, TEXT("UpdateAllPathsFromNode: Entrance node not fount."));
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
        UE_LOG(LogTemp, Log, TEXT("%d path founded."), AllPaths.Num());
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

            for (const UEdGraphPin* OtherPin : Pin->LinkedTo)
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
        for (const UEdGraphNode* Node : PastedNodes)
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
            Node->SetFlags(RF_Transactional);
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

void FProceduralLevelGraphEditor::PasteSelectedNodes(EMazeOrientation Direction)
{
    if (GraphEditorWidget.IsValid())
    {
        FString TextToImport;
        FPlatformApplicationMisc::ClipboardPaste(TextToImport);

        const FText TransactionTitle = (Direction == EMazeOrientation::Horizontal) ? 
            NSLOCTEXT("ProceduralLevelGraphEditor", "PasteMirrorH", "Paste Mirror Horizontal") :
            NSLOCTEXT("ProceduralLevelGraphEditor", "PasteMirrorV", "Paste Mirror Vertical");

        const FScopedTransaction Transaction(TransactionTitle);
        
        GraphAsset->EdGraph->Modify();
        const FVector2f PasteLocation = GraphEditorWidget->GetPasteLocation2f();
        TSet<UEdGraphNode*> PastedNodes;
        FEdGraphUtilities::ImportNodesFromText(GraphAsset->EdGraph, TextToImport, PastedNodes);

        FVector2D AvgNodePosition(0.0f, 0.0f);
        for (const UEdGraphNode* Node : PastedNodes)
        {
            AvgNodePosition.X += Node->NodePosX;
            AvgNodePosition.Y += Node->NodePosY;
        }

        if (PastedNodes.Num() > 0)
        {
            const float InvNumNodes = 1.0f / static_cast<float>(PastedNodes.Num());
            AvgNodePosition.X *= InvNumNodes;
            AvgNodePosition.Y *= InvNumNodes;
        }
        auto GetNodeSize = [](UEdGraphNode* InNode) -> FVector2f
        {
            float Width = 100.0f; 
            float Height = 100.0f;

            if (const URoomGraphNode* RoomRef = Cast<URoomGraphNode>(InNode))
            {
                bool bIsVert = RoomRef->RoomRotation == 0 || RoomRef->RoomRotation == 180;
                Width = (bIsVert ? RoomRef->RoomWidth : RoomRef->RoomHeight) * TILE_EDITOR_SCALE;
                Height = (bIsVert ? RoomRef->RoomHeight : RoomRef->RoomWidth) * TILE_EDITOR_SCALE;
            }
            else if (const UHallGraphNode* HallRef = Cast<UHallGraphNode>(InNode))
            {
                bool bIsVert = HallRef->RoomRotation == 0 || HallRef->RoomRotation == 180;
                float Length = HallRef->HallLength;
                float Thickness = HallRef->RoomWidth;
                
                Width = (bIsVert ? Thickness : Length) * TILE_EDITOR_SCALE;
                Height = (bIsVert ? Length : Thickness) * TILE_EDITOR_SCALE;
            }
            else if (const UEntranceGraphNode* EntranceRef = Cast<UEntranceGraphNode>(InNode))
            {
                bool bIsVert = EntranceRef->RoomRotation == 0 || EntranceRef->RoomRotation == 180;
                Width = (bIsVert ? EntranceRef->RoomWidth : EntranceRef->RoomHeight) * TILE_EDITOR_SCALE;
                Height = (bIsVert ? EntranceRef->RoomHeight : EntranceRef->RoomWidth) * TILE_EDITOR_SCALE;
            }
            else if (const URouterGraphNode* RouterRef = Cast<URouterGraphNode>(InNode))
            {
                bool bIsVert = RouterRef->RoomRotation == 0 || RouterRef->RoomRotation == 180;
                Width = (bIsVert ? RouterRef->RoomWidth : RouterRef->RoomHeight) * TILE_EDITOR_SCALE;
                Height = (bIsVert ? RouterRef->RoomHeight : RouterRef->RoomWidth) * TILE_EDITOR_SCALE;
            }
            else if (const ULayoutGraphNode* LayoutRef = Cast<ULayoutGraphNode>(InNode))
            {
                Width = LayoutRef->NodeWidth;
                Height = LayoutRef->NodeHeight;
            }
            return FVector2f(Width, Height);
            
        };
        
        for (UEdGraphNode* Node : PastedNodes)
        {
            Node->CreateNewGuid();
            const float RelativeX = Node->NodePosX - AvgNodePosition.X;
            const float RelativeY = Node->NodePosY - AvgNodePosition.Y;
            const FVector2f NodeSize = GetNodeSize(Node);
            if (Direction == EMazeOrientation::Horizontal)
            {
                Node->NodePosX = PasteLocation.X - RelativeX - NodeSize.X;
                Node->NodePosY = PasteLocation.Y + RelativeY;
                if (UMazeGraphNodeBase* MazeNode = Cast<UMazeGraphNodeBase>(Node))
                {
                    if (MazeNode->RoomRotation == 90) MazeNode->RoomRotation = 270;
                    else if (MazeNode->RoomRotation == 270) MazeNode->RoomRotation = 90;
                }
            }
            else 
            {
                Node->NodePosX = PasteLocation.X + RelativeX;
                Node->NodePosY = PasteLocation.Y - RelativeY - NodeSize.Y;
                if (UMazeGraphNodeBase* MazeNode = Cast<UMazeGraphNodeBase>(Node))
                {
                    if (MazeNode->RoomRotation == 0) MazeNode->RoomRotation = 180;
                    else if (MazeNode->RoomRotation == 180) MazeNode->RoomRotation = 0;
                }
            }
            Node->SnapToGrid(GRID_SNAP_SCALE);
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