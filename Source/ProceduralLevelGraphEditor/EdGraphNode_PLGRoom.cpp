#include "EdGraphNode_PLGRoom.h"

#include "GameplayTagContainer.h"
#include "EdGraph/EdGraphPin.h"
#include "Framework/Commands/GenericCommands.h"

#define LOCTEXT_NAMESPACE "EdGraphNode_PLGRoom"

FText UEdGraphNode_PLGRoom::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    return LOCTEXT("RoomNodeTitle", "Room");
}

void UEdGraphNode_PLGRoom::AllocateDefaultPins()
{
    //CreatePin(EGPD_Input, "Exec", FName(), TEXT("In"));
    //CreatePin(EGPD_Output, "Exec", FName(), TEXT("Out"));

    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, FName("North"));
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, FName("South"));
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, FName("East"));
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, FName("West"));
    
    FCreatePinParams PinParams;
    PinParams.bIsReference = true;

    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FGameplayTagContainer::StaticStruct(), TEXT("Gameplay Tag Container"), PinParams);
}

bool UEdGraphNode_PLGRoom::CanUserDeleteNode() const
{
    return true;
}

void UEdGraphNode_PLGRoom::GetNodeContextMenuActions(UToolMenu* Menu,
    UGraphNodeContextMenuContext* Context) const
{
    if (Context->Node)
    {
        FToolMenuSection& Section = Menu->AddSection("PLGSchemaNodeActions", NSLOCTEXT("ContextMenu", "NodeActionsMenuHeader", "Node Actions"));
       Section.AddMenuEntry(FGenericCommands::Get().Delete);
       Section.AddMenuEntry(FGenericCommands::Get().Duplicate);
    }
    Super::GetNodeContextMenuActions(Menu, Context);
}

#undef LOCTEXT_NAMESPACE
