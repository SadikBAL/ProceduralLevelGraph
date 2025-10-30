#include "ProceduralLevelGraphEditor.h"
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
#include "ProceduralLevelGraphRuntime/HallNode.h"
#include "ProceduralLevelGraphRuntime/RoomNode.h"
#include "ProceduralLevelGraphRuntime/RouterNode.h"
#include "Room/HallGraphNode.h"
#include "Room/MazeGraphNodeBase.h"
#include "Room/RoomGraphNode.h"
#include "Room/RouterGraphNode.h"

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
            GraphEditorWidget.ToSharedRef()
        ];
}

TSharedRef<SDockTab> FProceduralLevelGraphEditor::SpawnTab_Properties(const FSpawnTabArgs& Args)
{
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
                RuntimeRoom->RoomWidth = RoomEdNode->RoomWith;
                NewRuntimeNode = RuntimeRoom;
            }
            else if (UHallGraphNode* HallEdNode = Cast<UHallGraphNode>(EdNode))
            {
                UHallNode* RuntimeHall = NewObject<UHallNode>(GraphAsset);
                RuntimeHall->HallLength = HallEdNode->HallLength;
                RuntimeHall->bIsHorizontal = HallEdNode->bHorizentalMode;
                NewRuntimeNode = RuntimeHall;
            }
            else if (URouterGraphNode* RouterEdNode = Cast<URouterGraphNode>(EdNode))
            {
                URouterNode* RuntimeHall = NewObject<URouterNode>(GraphAsset);
                NewRuntimeNode = RuntimeHall;
            }
            
            if (NewRuntimeNode)
            {
                NewRuntimeNode->GameplayTags = MazeEdNode->GameplayTags;
                NewRuntimeNode->NodeGuid = MazeEdNode->NodeGuid;
                NodeMap.Add(EdNode, NewRuntimeNode);
                if (GraphAsset->StartNode == nullptr)
                {
                    GraphAsset->StartNode = NewRuntimeNode;
                }
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
        
        if (URoomGraphNode* RoomEdNode = Cast<URoomGraphNode>(EdGraph))
        {
            for (UEdGraphPin* LinkedPin : EdGraph->Pins)
            {
                if (LinkedPin->GetName() == FName("Up"))
                {
                    for (UEdGraphPin* LinkedTo : LinkedPin->LinkedTo)
                    {
                        if (NodeMap.Contains(LinkedTo->GetOwningNode()) && LinkedPin != LinkedTo)
                        {
                            RuntimeNode->UpNode = NodeMap[LinkedTo->GetOwningNode()];
                        }
                    }
                }
                else if (LinkedPin->GetName() == FName("Down"))
                {
                    for (UEdGraphPin* LinkedTo : LinkedPin->LinkedTo)
                    {
                        if (NodeMap.Contains(LinkedTo->GetOwningNode()) && LinkedPin != LinkedTo)
                        {
                            RuntimeNode->DownNode = NodeMap[LinkedTo->GetOwningNode()];
                        }
                    }
                }
                else if (LinkedPin->GetName() == FName("Left"))
                {
                    for (UEdGraphPin* LinkedTo : LinkedPin->LinkedTo)
                    {
                        if (NodeMap.Contains(LinkedTo->GetOwningNode()) && LinkedPin != LinkedTo)
                        {
                            RuntimeNode->LeftNode = NodeMap[LinkedTo->GetOwningNode()];
                        }
                    }
                }
                else if (LinkedPin->GetName() == FName("Right"))
                {
                    for (UEdGraphPin* LinkedTo : LinkedPin->LinkedTo)
                    {
                        if (NodeMap.Contains(LinkedTo->GetOwningNode()) && LinkedPin != LinkedTo)
                        {
                            RuntimeNode->RightNode = NodeMap[LinkedTo->GetOwningNode()];
                        }
                    }
                }
            }
        }
        else if (UHallGraphNode* HallEdNode = Cast<UHallGraphNode>(EdGraph))
        {
            for (UEdGraphPin* LinkedPin : EdGraph->Pins)
            {
                if (!HallEdNode->bHorizentalMode)
                {
                    if (LinkedPin->GetName() == FName("Up"))
                    {
                        for (UEdGraphPin* LinkedTo : LinkedPin->LinkedTo)
                        {
                            if (NodeMap.Contains(LinkedTo->GetOwningNode()) && LinkedPin != LinkedTo)
                            {
                                RuntimeNode->UpNode = NodeMap[LinkedTo->GetOwningNode()];
                            }
                        }
                    }
                    else if (LinkedPin->GetName() == FName("Down"))
                    {
                        for (UEdGraphPin* LinkedTo : LinkedPin->LinkedTo)
                        {
                            if (NodeMap.Contains(LinkedTo->GetOwningNode()) && LinkedPin != LinkedTo)
                            {
                                RuntimeNode->DownNode = NodeMap[LinkedTo->GetOwningNode()];
                            }
                        }
                    }
                }
                else
                {
                    if (LinkedPin->GetName() == FName("Up"))
                    {
                        for (UEdGraphPin* LinkedTo : LinkedPin->LinkedTo)
                        {
                            if (NodeMap.Contains(LinkedTo->GetOwningNode()) && LinkedPin != LinkedTo)
                            {
                                RuntimeNode->LeftNode = NodeMap[LinkedTo->GetOwningNode()];
                            }
                        }
                    }
                    else if (LinkedPin->GetName() == FName("Down"))
                    {
                        for (UEdGraphPin* LinkedTo : LinkedPin->LinkedTo)
                        {
                            if (NodeMap.Contains(LinkedTo->GetOwningNode()) && LinkedPin != LinkedTo)
                            {
                                RuntimeNode->RightNode = NodeMap[LinkedTo->GetOwningNode()];
                            }
                        }
                    }
                }
            }
        }
        else if (URouterGraphNode* RouterEdNode = Cast<URouterGraphNode>(EdGraph))
        {
            for (UEdGraphPin* LinkedPin : EdGraph->Pins)
            {
                if (LinkedPin->GetName() == FName("Up"))
                {
                    for (UEdGraphPin* LinkedTo : LinkedPin->LinkedTo)
                    {
                        if (NodeMap.Contains(LinkedTo->GetOwningNode()) && LinkedPin != LinkedTo)
                        {
                            RuntimeNode->UpNode = NodeMap[LinkedTo->GetOwningNode()];
                        }
                    }
                }
                else if (LinkedPin->GetName() == FName("Down"))
                {
                    for (UEdGraphPin* LinkedTo : LinkedPin->LinkedTo)
                    {
                        if (NodeMap.Contains(LinkedTo->GetOwningNode()) && LinkedPin != LinkedTo)
                        {
                            RuntimeNode->DownNode = NodeMap[LinkedTo->GetOwningNode()];
                        }
                    }
                }
                else if (LinkedPin->GetName() == FName("Left"))
                {
                    for (UEdGraphPin* LinkedTo : LinkedPin->LinkedTo)
                    {
                        if (NodeMap.Contains(LinkedTo->GetOwningNode()) && LinkedPin != LinkedTo)
                        {
                            RuntimeNode->LeftNode = NodeMap[LinkedTo->GetOwningNode()];
                        }
                    }
                }
                else if (LinkedPin->GetName() == FName("Right"))
                {
                    for (UEdGraphPin* LinkedTo : LinkedPin->LinkedTo)
                    {
                        if (NodeMap.Contains(LinkedTo->GetOwningNode()) && LinkedPin != LinkedTo)
                        {
                            RuntimeNode->RightNode = NodeMap[LinkedTo->GetOwningNode()];
                        }
                    }
                }
            }
        }
        GraphAsset->AllNodes.Add(RuntimeNode);
    }
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

void FProceduralLevelGraphEditor::OnSelectedNodesChanged(const TSet<class UObject*>& NewSelection)
{
	if (PropertyWidget.IsValid())
	{
		PropertyWidget->SetObjects(NewSelection.Array());
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