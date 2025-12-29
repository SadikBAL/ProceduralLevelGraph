#include "MazeNodeBase.h"

#include "ProceduralLevelGraphRuntime/ProceduralLevelGraphTypes.h"

void UMazeNodeBase::SpawnMazeObject(UWorld* World,FVector Position, EMazeDirection Direction)
{
	UE_LOG(LogTemp, Error, TEXT("UMazeNodeBase SpawnMazeObject: you could not spawn any object on base class."));
}

void UMazeNodeBase::SpawnMazeObjectFromNode(UWorld* World, FVector Position, EMazeDirection Direction,
	UMazeNodeBase* LinkedNode)
{
	bool bSpawned = false;
	if (LinkedNode)
	{
		for (auto Door : DoorData)
		{
			if (Door.LinkedNode == LinkedNode)
			{
				bSpawned = true;
				SpawnMazeObject(World, Position + (GetDoorOffset(Door) * - 1), Direction);
				break;
			}
		}
	}
	if (!bSpawned)
		SpawnMazeObject(World, Position, Direction);
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

EMazeDirection UMazeNodeBase::GetRotatedPinDirection(EMazeDirection Unrotated)
{
	int32 Value = static_cast<int32>(Unrotated);
	Value += (RoomRotation / 90);
	Value = Value % MAZE_DIRECTION_MAX;
	return static_cast<EMazeDirection>(Value);
}

FVector UMazeNodeBase::GetDoorPosition(EMazeDirection Direction, FDoorData Door)
{
	return GetEdgePosition(Direction) + GetDoorOffset(Door);
}

FVector UMazeNodeBase::GetDoorOffset(FDoorData Door)
{
		EMazeDirection DoorDirection = Door.DoorType;
		int32 LocalOffset = 0;
	    int32 LocalMult = 1;
	    if      (GetRotatedPinDirection(DoorDirection) == EMazeDirection::Up) {
		    //Up
		    LocalOffset = (DoorDirection == EMazeDirection::Left || DoorDirection == EMazeDirection::Right) 
		    ? Door.DoorOffset.Y : Door.DoorOffset.X;
			if (DoorDirection == EMazeDirection::Down 
				|| DoorDirection == EMazeDirection::Left)
			{
				LocalMult = -1;
			}
	    	return FVector(LocalOffset * LocalMult * TILE_SCALE,0.0f,0.0f);
	    }
	    else if (GetRotatedPinDirection(DoorDirection) == EMazeDirection::Right) {
		    //Right
		    LocalOffset = (DoorDirection == EMazeDirection::Left || DoorDirection == EMazeDirection::Right) 
			? Door.DoorOffset.Y : Door.DoorOffset.X;
		    if (DoorDirection == EMazeDirection::Down 
				||DoorDirection == EMazeDirection::Left)
		    {
	    		LocalMult = -1;
		    }
	    	return FVector(0.0f,LocalOffset * LocalMult * TILE_SCALE,0.0f);
	    }
	    else if (GetRotatedPinDirection(DoorDirection) == EMazeDirection::Down) {
	        //Left
	        LocalOffset = (DoorDirection == EMazeDirection::Left || DoorDirection == EMazeDirection::Right) 
			? Door.DoorOffset.Y : Door.DoorOffset.X;
	        if (DoorDirection == EMazeDirection::Right 
				||DoorDirection == EMazeDirection::Up)
	        {
		        LocalMult = -1;
	        }
	    	return FVector(LocalOffset * LocalMult * TILE_SCALE,0.0f,0.0f);
	    }
	    else if (GetRotatedPinDirection(DoorDirection) == EMazeDirection::Left) {
	        //Down
	        LocalOffset = (DoorDirection == EMazeDirection::Left || DoorDirection == EMazeDirection::Right) 
			? Door.DoorOffset.Y : Door.DoorOffset.X;
	        if (DoorDirection == EMazeDirection::Right
		        || DoorDirection == EMazeDirection::Up)
	        {
		        LocalMult = -1;
	        }
	    	return FVector(0.0f,LocalOffset * LocalMult * TILE_SCALE,0.0f);
	    }
	return FVector(0.0f,0.0f,0.0f);
}

