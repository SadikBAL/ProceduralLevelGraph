#include "ProceduralLevelGraphEditor.h"
#include "ProceduralLevelGraphRuntime/MazeGraph.h"
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

void FProceduralLevelGraphEditor::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, UMazeGraph* InGraph)
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

TSharedRef<SDockTab> FProceduralLevelGraphEditor::SpawnTab_GraphCanvas(const FSpawnTabArgs& Args)
{
    SGraphEditor::FGraphEditorEvents InEvents;
    InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FProceduralLevelGraphEditor::OnSelectedNodesChanged);

    GraphEditorWidget = SNew(SGraphEditor)
        .AdditionalCommands(GetToolkitCommands())
        .GraphToEdit(GraphAsset->EdGraph)
        .GraphEvents(InEvents)
        .AdditionalCommands(CommandList);
        //.GraphEditorEvents(InEvents);

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

void FProceduralLevelGraphEditor::OnGraphChanged(const FEdGraphEditAction& Action)
{
    if (Action.Action == GRAPHACTION_AddNode || Action.Action == GRAPHACTION_RemoveNode)
    {
        GraphAsset->MarkPackageDirty();
    }
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