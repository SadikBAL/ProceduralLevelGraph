#include "HallNode.h"

UHallNode::UHallNode()
{
	static ConstructorHelpers::FClassFinder<AActor> BP_Room_ClassFinder(
	TEXT("Blueprint'/Game/LevelPrototyping/BP_Hall.BP_Hall_C'")
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

float UHallNode::GetHalfDistanceOfRoom(EMazeOrientation Orientation)
{
	if (Orientation == EMazeOrientation::Horizontal)
	{
		return 40 * 0.5;
	}
	else
	{
		return 100 * HallLength * 0.5;
	}
}
