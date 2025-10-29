#include "RouterNode.h"

URouterNode::URouterNode()
{
	static ConstructorHelpers::FClassFinder<AActor> BP_Room_ClassFinder(
	TEXT("Blueprint'/Game/LevelPrototyping/BP_Router.BP_Router_C'")
);
	if (BP_Room_ClassFinder.Succeeded())
	{
		ActorToSpawnClass = BP_Room_ClassFinder.Class;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("URouterNode constructor: BP_Router sınıfı bulunamadı!"));
	}
}



float URouterNode::GetHalfDistanceOfRoom(EMazeOrientation Orientation)
{
	return 200.0f;
}
