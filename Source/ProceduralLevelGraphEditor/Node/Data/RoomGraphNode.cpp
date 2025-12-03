#include "RoomGraphNode.h"
#include "EdGraph/EdGraphPin.h"

#define LOCTEXT_NAMESPACE "RoomGraphNode"

URoomGraphNode::URoomGraphNode()
{
    if (RoomLevelInstanceRefs.IsEmpty())
    {
        static ConstructorHelpers::FClassFinder<ARoomLevelInstance> BP_LevelInstance_Finder(
        TEXT("Blueprint'/Game/LevelPrototyping/MazeLevelInstances/Rooms/LI_Room_12X8.LI_Room_12X8_C'")
        );
        if (BP_LevelInstance_Finder.Succeeded())
        {
            RoomLevelInstanceRefs.Add(BP_LevelInstance_Finder.Class);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("URoomGraphNode constructor: LI_Room_12X8 class not found!"));
        }
        static ConstructorHelpers::FClassFinder<AHallLevelInstance> BP_HallInstance_Finder(
        TEXT("Blueprint'/Game/LevelPrototyping/MazeLevelInstances/Halls/LI_Hall_4X1.LI_Hall_4X1_C'")
        );
        if (BP_HallInstance_Finder.Succeeded())
        {
            HallLevelInstanceRefs.Add(BP_HallInstance_Finder.Class);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("URoomGraphNode constructor: LI_Room_12X8 class not found!"));
        }
    }
    OnTileBlueprintsChanged();
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
