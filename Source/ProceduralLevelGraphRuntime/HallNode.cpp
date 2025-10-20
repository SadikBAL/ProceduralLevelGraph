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

UHallNode::~UHallNode()
{
	
}

float UHallNode::GetHalfDistanceOfRoom(EMazeOrientation Orientation)
{
	return 100 * HallLength * 0.5;
}

