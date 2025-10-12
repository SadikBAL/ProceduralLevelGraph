#include "RoomGraphNodeBase.h"

#include "Framework/Commands/GenericCommands.h"
#define LOCTEXT_NAMESPACE "RoomGraphNodeBase"
FText URoomGraphNodeBase::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("RoomNodeTitle", "Room");
}

bool URoomGraphNodeBase::CanUserDeleteNode() const
{
	return true;
}

void URoomGraphNodeBase::GetNodeContextMenuActions(class UToolMenu* Menu,
                                                   class UGraphNodeContextMenuContext* Context) const
{
	if (Context->Node)
	{
		FToolMenuSection& Section = Menu->AddSection("PLGSchemaNodeActions", NSLOCTEXT("ContextMenu", "NodeActionsMenuHeader", "Node Actions"));
		Section.AddMenuEntry(FGenericCommands::Get().Delete);
	}
	Super::GetNodeContextMenuActions(Menu, Context);
}
