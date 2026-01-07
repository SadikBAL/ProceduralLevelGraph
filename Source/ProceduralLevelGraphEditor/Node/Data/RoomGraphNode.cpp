#include "RoomGraphNode.h"

#include "HallGraphNode.h"
#include "EdGraph/EdGraphPin.h"

#define LOCTEXT_NAMESPACE "RoomGraphNode"

URoomGraphNode::URoomGraphNode()
{
    OnTileBlueprintsChanged();
}

FText URoomGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    return LOCTEXT("RoomNodeTitle", "Room");
}

void URoomGraphNode::AllocateDefaultPins()
{
    FCreatePinParams PinParams;
    for (int i = 0; i < DoorData.Num(); i++)
    {
        FName PinName;
        switch (DoorData[i].DoorLocation)
        {
            case EMazeDirection::Up:    PinName = FName(*FString::Printf(TEXT("Up_%d_Pin"), i)); break;
            case EMazeDirection::Down:  PinName = FName(*FString::Printf(TEXT("Down_%d_Pin"), i)); break;
            case EMazeDirection::Left:  PinName = FName(*FString::Printf(TEXT("Left_%d_Pin"), i)); break;
            case EMazeDirection::Right: PinName = FName(*FString::Printf(TEXT("Right_%d_Pin"), i)); break;
            default: continue;
        }
        if (FindPin(PinName)) continue;

        PinParams.Index = i;
        UEdGraphPin* TempPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, PinName, PinParams);
        //Pins.Add(TempPin);
        if (DoorData[i].DoorStatus == EMazePinType::Hidden && TempPin != nullptr)
        {
            TempPin->bHidden = true;
        }
    }
}

void URoomGraphNode::OnTileBlueprintsChanged()
{
    Modify();
    for (int32 i = Pins.Num() - 1; i >= 0; --i)
    {
        if (UEdGraphPin* Pin = Pins[i])
        {
            Pin->BreakAllPinLinks();
        }
    }
    for (int32 i = Pins.Num() - 1; i >= 0; --i)
    {
        RemovePin(Pins[i]);
    }
    DoorData.Empty();
    RoomWidth = EMPTY_SIZE;
    RoomHeight = EMPTY_SIZE;
    for (const TSubclassOf<ARoomLevelInstance>& TileClass : RoomLevelInstanceRef)
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
                DoorData.Append(DefaultTile->DoorData);
            }
        }
    }
    AllocateDefaultPins();
    if (HasAnyFlags(RF_ClassDefaultObject | RF_NeedLoad))
    {
        return;
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
    if (PropertyName == GET_MEMBER_NAME_CHECKED(URoomGraphNode, RoomLevelInstanceRef))
    {
        for (const TSubclassOf<ARoomLevelInstance>& TileClass : RoomLevelInstanceRef)
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
    for (const TSubclassOf<ARoomLevelInstance>& TileClass : RoomLevelInstanceRef)
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
    if (Index >= 0 && Index < DoorData.Num())
    {
        return GetRotatedPinDirection(DoorData[Index].DoorLocation);
    }
    return EMazeDirection::None;
}

EMazeFloor URoomGraphNode::GetMazePinFloor(const UEdGraphPin* Pin)
{
    int32 Index = GetIndexFromPinName(Pin->PinName);
    if (Index >= 0 && Index < DoorData.Num())
    {
        return AddFloor(this->RoomFloor,DoorData[Index].DoorFloor);
    }
    return EMazeFloor::Error;
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

FText URoomGraphNode::GetNodeName() const
{
    return FText::FromString("ROOM");
}

FSlateColor URoomGraphNode::GetNodeBackgroundColor() const
{
    if (DoorData.Num() > 0)
        return FSlateColor(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f));
    return FSlateColor(FLinearColor(1.0f, 0.1f, 0.1f, 0.6f));
}

#undef LOCTEXT_NAMESPACE
