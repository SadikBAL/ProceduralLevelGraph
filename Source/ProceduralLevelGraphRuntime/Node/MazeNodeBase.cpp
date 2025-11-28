#include "MazeNodeBase.h"

#include "ProceduralLevelGraphRuntime/ProceduralLevelGraphTypes.h"

void UMazeNodeBase::SpawnMazeObject(UWorld* World,FVector Position, EMazeDirection Direction)
{
	UE_LOG(LogTemp, Error, TEXT("UMazeNodeBase SpawnMazeObject: you could not spawn any object on base class."));
}

FVector UMazeNodeBase::GetEdgePosition(EMazeDirection Direction)
{
	switch (Direction)
	{
	case EMazeDirection::Up:
		return SpawnLocation + FVector(0, GetHalfDistanceOfRoom(EMazeOrientation::Vertical) * -1, 0);
	case EMazeDirection::Down:
		return SpawnLocation + FVector(0, GetHalfDistanceOfRoom(EMazeOrientation::Vertical), 0);
	case EMazeDirection::Left:
		return SpawnLocation + FVector(GetHalfDistanceOfRoom(EMazeOrientation::Horizontal) * -1, 0, 0);
	case EMazeDirection::Right:
		return SpawnLocation + FVector(GetHalfDistanceOfRoom(EMazeOrientation::Horizontal), 0, 0);
	default:
		return SpawnLocation;
	}
}

float UMazeNodeBase::GetHalfDistanceOfRoom(EMazeOrientation Orientation)
{
	return 0.0f;
}
