#include "MazeGraphNodeBase.h"

#include "Framework/Commands/GenericCommands.h"
#define LOCTEXT_NAMESPACE "RoomGraphNodeBase"
FText UMazeGraphNodeBase::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("RoomNodeTitle", "Room");
}

bool UMazeGraphNodeBase::CanUserDeleteNode() const
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
	}
	Super::GetNodeContextMenuActions(Menu, Context);
}

