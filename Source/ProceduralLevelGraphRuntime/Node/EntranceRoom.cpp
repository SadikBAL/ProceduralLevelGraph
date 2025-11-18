#include "EntranceRoom.h"

#include "RoomNode.h"

UEntranceNode::UEntranceNode()
{
	static ConstructorHelpers::FClassFinder<AActor> BP_Entrance_ClassFinder(
		TEXT("Blueprint'/Game/LevelPrototyping/BP_Entrance.BP_Entrance_C'")
		
	);
	if (BP_Entrance_ClassFinder.Succeeded())
	{
		ActorToSpawnClass = BP_Entrance_ClassFinder.Class;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UEntranceNode constructor: BP_Entrance_C sınıfı bulunamadı!"));
	}
}

float UEntranceNode::GetHalfDistanceOfRoom(EMazeOrientation Orientation)
{
	if (Orientation == EMazeOrientation::Horizontal)
	{
		return RoomWidth * TILE_SCALE * 0.5;
	}
	else
	{
		return RoomHeight * TILE_SCALE * 0.5;
	}

}

