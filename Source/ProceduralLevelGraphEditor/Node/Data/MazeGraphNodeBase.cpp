#include "MazeGraphNodeBase.h"

#include "Framework/Commands/GenericCommands.h"
#include "ProceduralLevelGraphRuntime/ProceduralLevelGraphTypes.h"
#define LOCTEXT_NAMESPACE "MazeGraphNodeBase"

FText UMazeGraphNodeBase::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("RoomNodeTitle", "Room");
}

bool UMazeGraphNodeBase::CanUserDeleteNode() const
{
	return true;
}

bool UMazeGraphNodeBase::CanUserCopyNode() const
{
	return true;
}

void UMazeGraphNodeBase::GetNodeContextMenuActions(class UToolMenu* Menu,
                                                   class UGraphNodeContextMenuContext* Context) const
{
	if (Context->Node)
	{
		FToolMenuSection& Section = Menu->AddSection("PLGSchemaNodeActions", NSLOCTEXT("ContextMenu", "NodeActionsMenuHeader", "Node Actions"));
		Section.AddMenuEntry(FGenericCommands::Get().Delete);
		Section.AddMenuEntry(FGenericCommands::Get().Copy);
		Section.AddMenuEntry(FGenericCommands::Get().Duplicate);
	}
	Super::GetNodeContextMenuActions(Menu, Context);
}

void UMazeGraphNodeBase::PinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::PinConnectionListChanged(Pin);
	if (UEdGraph* Graph = GetGraph())
	{
		Graph->GetOuter()->MarkPackageDirty();
	}
}

void UMazeGraphNodeBase::RotateRoomToLeft()
{
	RoomRotation -= 90;
	if (RoomRotation < 0)
		RoomRotation = 270;
    
	if (UEdGraph* MyGraph = GetGraph())
	{
		MyGraph->NotifyGraphChanged();
	}
}

EMazePinType UMazeGraphNodeBase::GetPinType()
{
	return EMazePinType::Tier1;
}

void UMazeGraphNodeBase::RotateRoomToRight()
{
	RoomRotation += 90;
	RoomRotation = RoomRotation % 360;
	if (UEdGraph* MyGraph = GetGraph())
	{
		MyGraph->NotifyGraphChanged();
	}
}

#undef LOCTEXT_NAMESPACE

