#include "RoomGraphNode.h"

#include "HallGraphNode.h"
#include "EdGraph/EdGraphPin.h"

#define LOCTEXT_NAMESPACE "RoomGraphNode"

URoomGraphNode::URoomGraphNode()
{
    //OnTileBlueprintsChanged();
}

FText URoomGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    return LOCTEXT("RoomNodeTitle", "Room");
}

void URoomGraphNode::AllocateDefaultPins()
{
    Pins.Empty();
    FCreatePinParams PinParams;
    for (int i = 0; i <  DoorDatas.Num(); i++)
    {
        PinParams.Index = i;
        FName PinName;
        UEdGraphPin* TempPin = nullptr;
        switch (DoorDatas[i].DoorType)
        {
            case EMazeDirection::Up:
            PinName = FName(*FString::Printf(TEXT("Up_%d_Pin"), i));
            TempPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, PinName,PinParams);
            break;
            case EMazeDirection::Down:
            PinName = FName(*FString::Printf(TEXT("Down_%d_Pin"), i));
            TempPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, PinName,PinParams);
            break;
            case EMazeDirection::Left:
            PinName = FName(*FString::Printf(TEXT("Left_%d_Pin"), i));
            TempPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, PinName,PinParams);
            break;
            case EMazeDirection::Right:
            PinName = FName(*FString::Printf(TEXT("Right_%d_Pin"), i));
            TempPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, PinName,PinParams);
            break;
            default:
            break;
        }
        if (DoorDatas[i].DoorVisibility == EMazePinType::Hidden && TempPin != nullptr)
        {
            TempPin->bHidden = true;
        }
    }
}

void URoomGraphNode::OnTileBlueprintsChanged()
{
    //Update Width and Height
    DoorDatas.Empty();
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
                DoorDatas.Append(DefaultTile->DoorDatas);
            }
        }
    }
    if (UEdGraph* MyGraph = GetGraph())
    {
        MyGraph->NotifyGraphChanged();
    }
}

void URoomGraphNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    const FName PropertyName = (PropertyChangedEvent.Property != nullptr) 
                       ? PropertyChangedEvent.Property->GetFName() 
                       : NAME_None;
    if (PropertyName == GET_MEMBER_NAME_CHECKED(URoomGraphNode, RoomLevelInstanceRefs))
    {
        for (const TSubclassOf<ARoomLevelInstance>& TileClass : RoomLevelInstanceRefs)
        {
            if (TileClass)
            {
                AMazeTileLevelInstance* MutableActor = const_cast<AMazeTileLevelInstance*>(GetDefault<AMazeTileLevelInstance>(TileClass));
                MutableActor->OnMazeTileLevelInstanceUpdated.RemoveAll(this);
                MutableActor->OnMazeTileLevelInstanceUpdated.AddUObject(this, &URoomGraphNode::OnTileBlueprintsChanged);
            }
        }
        OnTileBlueprintsChanged();
    }
    Super::PostEditChangeProperty(PropertyChangedEvent);
}

void URoomGraphNode::PostLoad()
{
    Super::PostLoad();
    for (const TSubclassOf<ARoomLevelInstance>& TileClass : RoomLevelInstanceRefs)
    {
        if (TileClass)
        {
            AMazeTileLevelInstance* MutableActor = const_cast<AMazeTileLevelInstance*>(GetDefault<AMazeTileLevelInstance>(TileClass));
            MutableActor->OnMazeTileLevelInstanceUpdated.RemoveAll(this);
            MutableActor->OnMazeTileLevelInstanceUpdated.AddUObject(this, &URoomGraphNode::OnTileBlueprintsChanged);
        }
    }
}

EMazeDirection URoomGraphNode::GetMazePinDirection(const UEdGraphPin* Pin)
{
    int32 Index = GetIndexFromPinName(Pin->PinName);
    if (Index >= 0 && Index < DoorDatas.Num())
    {
        return GetRotatedPinDirection(DoorDatas[Index].DoorType);
    }
    return EMazeDirection::None;
}
int32 URoomGraphNode::GetIndexFromPinName(const FName& PinName)
{
    FString PinNameStr = PinName.ToString();
    TArray<FString> Parts;
    PinNameStr.ParseIntoArray(Parts, TEXT("_"), true);
    if (Parts.Num() >= 2 && Parts[1].IsNumeric())
    {
        return FCString::Atoi(*Parts[1]);
    }
    return INDEX_NONE;
}

#undef LOCTEXT_NAMESPACE
