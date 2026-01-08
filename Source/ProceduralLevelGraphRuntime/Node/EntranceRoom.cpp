#include "EntranceRoom.h"

#include "RoomNode.h"
#include "Engine/LevelStreamingDynamic.h"
#include "ProceduralLevelGraphRuntime/ProceduralLevelGraphTypes.h"

UEntranceNode::UEntranceNode() : URoomNode()
{
	
}

FVector UEntranceNode::GetRoomPosition() const
{
	return FVector( RoomPosition.X, RoomPosition.Y, GetFloorHeight(RoomFloor));
}
