#include "RoomNode.h"

URoomNode::URoomNode()
{
	static ConstructorHelpers::FClassFinder<AActor> BP_Room_ClassFinder(
		TEXT("Blueprint'/Game/LevelPrototyping/BP_Room.BP_Room_C'")
	);
	if (BP_Room_ClassFinder.Succeeded())
	{
		ActorToSpawnClass = BP_Room_ClassFinder.Class;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("URoomNode constructor: BP_Room_C sınıfı bulunamadı!"));
	}
}

float URoomNode::GetHalfDistanceOfRoom(EMazeOrientation Orientation)
{
	if (Orientation == EMazeOrientation::Horizontal)
	{
		return RoomWidth * 0.5;
	}
	else
	{
		return RoomHeight * 0.5;
	}

}
