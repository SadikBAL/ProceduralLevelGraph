#include "RoomGraphNode.h"
#include "EdGraph/EdGraphPin.h"

#define LOCTEXT_NAMESPACE "RoomGraphNode"

URoomGraphNode::URoomGraphNode()
{
    if (RoomLevelInstanceRefs.IsEmpty())
    {
    }
    OnTileBlueprintsChanged();
}

FText URoomGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    return LOCTEXT("RoomNodeTitle", "Room");
}

void URoomGraphNode::AllocateDefaultPins()
{
    /*
        CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, FName("Up"));
        CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, FName("Down"));
        CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, FName("Left"));
        CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, FName("Right"));
    */
    FCreatePinParams PinParams;
    for (int i = 0; i <  DoorDatas.Num(); i++)
    {
        if (DoorDatas[i].DoorVisibility == EMazePinType::Tier1)
        {
            PinParams.Index =i;
            switch (DoorDatas[i].DoorType)
            {
                case EMazeDirection::Up:
                CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, FName("Up"),PinParams);
                break;
                case EMazeDirection::Down:
                CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, FName("Down"),PinParams);
                break;
                case EMazeDirection::Left:
                CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, FName("Left"),PinParams);
                break;
                case EMazeDirection::Right:
                CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, FName("Right"),PinParams);
                break;
                default:
                break;
            }
            
        }
    }
}

void URoomGraphNode::OnTileBlueprintsChanged()
{
    //Update Width and Height
    RoomWidth = 0;
    RoomHeight = 0;
    for (const TSubclassOf<ARoomLevelInstance>& TileClass : RoomLevelInstanceRefs)
    {
        if (TileClass)
        {
            const AMazeTileLevelInstance* DefaultTile = GetDefault<AMazeTileLevelInstance>(TileClass);
            if (DefaultTile)
            {
                if (DefaultTile->Width > RoomWidth )
                {
                    RoomWidth = DefaultTile->Width;
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
    const FName PropertyName = (PropertyChangedEvent.Property != nullptr) 
                       ? PropertyChangedEvent.Property->GetFName() 
                       : NAME_None;
    if (PropertyName == GET_MEMBER_NAME_CHECKED(URoomGraphNode, RoomLevelInstanceRefs))
    {
        OnTileBlueprintsChanged();
    }
    Super::PostEditChangeProperty(PropertyChangedEvent);
}

#undef LOCTEXT_NAMESPACE
