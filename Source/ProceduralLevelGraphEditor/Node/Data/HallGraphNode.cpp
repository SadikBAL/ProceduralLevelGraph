#include "HallGraphNode.h"
#include "EdGraph/EdGraphPin.h"

#define LOCTEXT_NAMESPACE "HallGraphNode"

UHallGraphNode::UHallGraphNode()
{
	HallLength = 5.0f;
	FDoorData UpDoor;
	UpDoor.DoorLocation = EMazeDirection::Up;
	UpDoor.DoorStatus = EMazePinType::Closed;
	UpDoor.DoorFloor = EMazeFloor::Floor0;
	UpDoor.DoorOffset = FVector2D(0,0);
	DoorData.Add(UpDoor);
	FDoorData DownDoor;
	DownDoor.DoorLocation = EMazeDirection::Down;
	DownDoor.DoorStatus = EMazePinType::Closed;
	DownDoor.DoorOffset = FVector2D(0,0);
	DownDoor.DoorFloor = EMazeFloor::Floor0;
	DoorData.Add(DownDoor);
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
	AllocateDefaultPins();
	OnHallDataAssetChanged();
}

FText UHallGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("HallNodeTitle", "Hall");
}

void UHallGraphNode::AllocateDefaultPins()
{
	UEdGraphPin* TempPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, FName("Up"));
	//Pins.Add(TempPin);
	TempPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, FName("Down"));
	//Pins.Add(TempPin);
}

void UHallGraphNode::PostLoad()
{
	Super::PostLoad();
	if (HallData)
	{
		HallData->OnHallDataChanged.RemoveAll(this);
		HallData->OnHallDataChanged.AddUObject(this, &UHallGraphNode::OnHallDataAssetChanged);
	}
}

FText UHallGraphNode::GetNodeName() const
{
	return FText::FromString("HALL");
}

FSlateColor UHallGraphNode::GetNodeBackgroundColor() const
{
	if (bHallData)
		return FSlateColor(FLinearColor(0.8f, 0.4f, 0.4f, 0.4f));
	return FSlateColor(FLinearColor(1.0f, 0.1f, 0.1f, 0.6f));
}

EMazeDirection UHallGraphNode::GetMazePinDirection(const UEdGraphPin* Pin)
{
	if (Pin->GetName().StartsWith(TEXT("Up")))
	{
		return GetRotatedPinDirection(DoorData[0].DoorLocation);
	}
	else if (Pin->GetName().StartsWith(TEXT("Down")))
	{
		return GetRotatedPinDirection(DoorData[1].DoorLocation);
	}
	return EMazeDirection::None;
}

EMazeFloor UHallGraphNode::GetMazePinFloor(const UEdGraphPin* Pin)
{
	if (Pin->GetName().StartsWith(TEXT("Up")))
	{
		return AddFloor(this->RoomFloor,DoorData[0].DoorFloor);
	}
	else if (Pin->GetName().StartsWith(TEXT("Down")))
	{
		return AddFloor(this->RoomFloor,DoorData[0].DoorFloor);
	}
	return EMazeFloor::Error;
}

void UHallGraphNode::OnHallDataAssetChanged()
{
	bHallData = false;
	RoomWidth = EMPTY_SIZE;
	if (HallData)
	{
		if (HallData->HallCapTile)
		{
			if (HallData->HallCapTile->GetDefaultObject<AMazeTileLevelInstance>()->Width > RoomWidth)
			{
				RoomWidth = HallData->HallCapTile->GetDefaultObject<AMazeTileLevelInstance>()->Width;
			}
		}
		if (HallData->HallTiles.Num() > 0)
		{
			bHallData = true;
			for (auto Element : HallData->HallTiles)
			{
				if (Element && Element->GetDefaultObject<AMazeTileLevelInstance>()->Width > RoomWidth)
				{
					RoomWidth = Element->GetDefaultObject<AMazeTileLevelInstance>()->Width;
				}
			}
		}
	}
	if (HasAnyFlags(RF_ClassDefaultObject | RF_NeedLoad))
	{
		return;
	}
	if (UEdGraph* MyGraph = GetGraph())
	{
		MyGraph->NotifyGraphChanged();
	}
}

void UHallGraphNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UHallGraphNode, HallData))
	{
		if (HallData)
		{
			HallData->OnHallDataChanged.RemoveAll(this);
			HallData->OnHallDataChanged.AddUObject(this, &UHallGraphNode::OnHallDataAssetChanged);
			OnHallDataAssetChanged();
		}
	}
}

void UHallGraphNode::UpdateHallLength(int Value)
{
	HallLength = Value;
	if (UEdGraph* MyGraph = GetGraph())
	{
		MyGraph->NotifyGraphChanged();
	}
}

#undef LOCTEXT_NAMESPACE
