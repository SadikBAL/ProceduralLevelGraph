#include "HallGraphNode.h"
#include "EdGraph/EdGraphPin.h"

#define LOCTEXT_NAMESPACE "HallGraphNode"

UHallGraphNode::UHallGraphNode()
{
	HallLength = 5.0f;
	OnHallDataAssetChanged();
	
}

FText UHallGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("HallNodeTitle", "Hall");
}

void UHallGraphNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, FName("Up"));
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, FName("Down"));
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, FName("Left"));
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, FName("Right"));
}

EMazePinType UHallGraphNode::GetPinType()
{
	return Super::GetPinType();
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

void UHallGraphNode::OnHallDataAssetChanged()
{
	if (HallData)
	{
		RoomWidth = 0;
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
			for (auto Element : HallData->HallTiles)
			{
				if (Element->GetDefaultObject<AMazeTileLevelInstance>()->Width > RoomWidth)
				{
					RoomWidth = Element->GetDefaultObject<AMazeTileLevelInstance>()->Width;
				}
			}
		}
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

void UHallGraphNode::RefreshNode()
{
	OnHallDataAssetChanged();
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
