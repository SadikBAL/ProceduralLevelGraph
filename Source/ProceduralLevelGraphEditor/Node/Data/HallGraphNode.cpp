#include "HallGraphNode.h"
#include "EdGraph/EdGraphPin.h"

#define LOCTEXT_NAMESPACE "HallGraphNode"

UHallGraphNode::UHallGraphNode()
{
	HallLength = 5.0f;
	FDoorData UpDoor;
	UpDoor.DoorType = EMazeDirection::Up;
	UpDoor.DoorVisibility = EMazePinType::Tier1;
	UpDoor.DoorOffset = FVector2D(0,0);
	DoorData.Add(UpDoor);
	FDoorData DownDoor;
	DownDoor.DoorType = EMazeDirection::Down;
	DownDoor.DoorVisibility = EMazePinType::Tier1;
	DownDoor.DoorOffset = FVector2D(0,0);
	DoorData.Add(DownDoor);
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
		return GetRotatedPinDirection(DoorData[0].DoorType);
	}
	else if (Pin->GetName().StartsWith(TEXT("Down")))
	{
		return GetRotatedPinDirection(DoorData[1].DoorType);
	}
	return EMazeDirection::None;
}

void UHallGraphNode::OnHallDataAssetChanged()
{
	bHallData = false;
	RoomWidth = EMPTY_SIZE;
	if (HallData)
	{
		if (HallData->HallEndTile)
		{
			if (HallData->HallEndTile->GetDefaultObject<AMazeTileLevelInstance>()->Width > RoomWidth)
			{
				RoomWidth = HallData->HallEndTile->GetDefaultObject<AMazeTileLevelInstance>()->Width;
			}
		}
		if (HallData->HallStartTile)
		{
			if (HallData->HallStartTile->GetDefaultObject<AMazeTileLevelInstance>()->Width > RoomWidth)
			{
				RoomWidth = HallData->HallStartTile->GetDefaultObject<AMazeTileLevelInstance>()->Width;
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
