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
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, FName("North"));
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, FName("South"));
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, FName("East"));
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, FName("West"));
    
    FCreatePinParams PinParams;
    PinParams.bIsReference = true;

    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FGameplayTagContainer::StaticStruct(), TEXT("Gameplay Tag Container"), PinParams);
}
#undef LOCTEXT_NAMESPACE
