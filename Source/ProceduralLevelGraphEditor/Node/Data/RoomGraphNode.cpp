#include "RoomGraphNode.h"
#include "EdGraph/EdGraphPin.h"

#define LOCTEXT_NAMESPACE "RoomGraphNode"

URoomGraphNode::URoomGraphNode()
{
    RoomHeight = 5;
    RoomWith = 5;
}

FText URoomGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    return LOCTEXT("RoomNodeTitle", "Room");
}
void URoomGraphNode::AllocateDefaultPins()
{
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, FName("Up"));
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, FName("Down"));
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, FName("Left"));
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, FName("Right"));
}
void URoomGraphNode::OnTileBlueprintsChanged()
{
    //Update Witdh and Heighht
    RoomWith = 0;
    RoomHeight = 0;
    for (const TSubclassOf<AMazeTileActor>& TileClass : TileBlueprints)
    {
        if (TileClass)
        {
            const AMazeTileActor* DefaultTile = GetDefault<AMazeTileActor>(TileClass);
            if (DefaultTile)
            {
                if (DefaultTile->With > RoomWith )
                {
                    RoomWith = DefaultTile->With;
                }
                if (DefaultTile->Height > RoomHeight)
                {
                    RoomHeight = DefaultTile->Height;
                }
            }
        }
    }
}

void URoomGraphNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    FName PropertyName = (PropertyChangedEvent.Property != nullptr) 
                       ? PropertyChangedEvent.Property->GetFName() 
                       : NAME_None;
    if (PropertyName == GET_MEMBER_NAME_CHECKED(URoomGraphNode, TileBlueprints))
    {
        OnTileBlueprintsChanged();
    }
    Super::PostEditChangeProperty(PropertyChangedEvent);
}
#undef LOCTEXT_NAMESPACE
