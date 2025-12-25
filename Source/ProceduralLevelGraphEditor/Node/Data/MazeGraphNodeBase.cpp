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

EMazeDirection UMazeGraphNodeBase::GetMazePinDirection(const UEdGraphPin* Pin)
{
	return EMazeDirection::None;
}

void UMazeGraphNodeBase::RotateRoomToLeft()
{
	RoomRotation -= 90;
	if (RoomRotation < 0)
		RoomRotation = 270;
	for (auto& Pin : Pins)
	{
		Pin->BreakAllPinLinks();
	}
	if (UEdGraph* MyGraph = GetGraph())
	{
		MyGraph->NotifyGraphChanged();
	}
}

EMazePinType UMazeGraphNodeBase::GetPinType()
{
	return EMazePinType::Tier1;
}

EMazeDirection UMazeGraphNodeBase::GetRotatedPinDirection(EMazeDirection Unrotated)
{
	int32 Value = static_cast<int32>(Unrotated);
	Value += (RoomRotation / 90);
	Value = Value % MAZE_DIRECTION_MAX;
	return static_cast<EMazeDirection>(Value);
}

FText UMazeGraphNodeBase::GetNodeName() const
{
	return FText::FromString("BASE");
}

FSlateColor UMazeGraphNodeBase::GetNodeBackgroundColor() const
{
	return FSlateColor(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f));
}

void UMazeGraphNodeBase::RotateRoomToRight()
{
	RoomRotation += 90;
	RoomRotation = RoomRotation % 360;
	for (auto& Pin : Pins)
	{
		Pin->BreakAllPinLinks();
	}
	if (UEdGraph* MyGraph = GetGraph())
	{
		MyGraph->NotifyGraphChanged();
	}
}

#undef LOCTEXT_NAMESPACE

