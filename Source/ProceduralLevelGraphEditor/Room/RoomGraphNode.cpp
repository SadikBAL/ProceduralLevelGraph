#include "RoomGraphNode.h"

#include "GameplayTagContainer.h"
#include "EdGraph/EdGraphPin.h"
#include "Framework/Commands/GenericCommands.h"

#define LOCTEXT_NAMESPACE "RoomGraphNode"

FText URoomGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    return LOCTEXT("RoomNodeTitle", "Room");
}
void URoomGraphNode::AllocateDefaultPins()
{
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Delegate, FName("Up"));
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Delegate, FName("Down"));
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Delegate, FName("Left"));
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Delegate, FName("Right"));
    
    FCreatePinParams PinParams;
    PinParams.bIsReference = true;

    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FGameplayTagContainer::StaticStruct(), TEXT("Gameplay Tag Container"), PinParams);
}
#undef LOCTEXT_NAMESPACE
