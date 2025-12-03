#include "HallNode.h"

#include "ProceduralLevelGraphRuntime/ProceduralLevelGraphTypes.h"

UHallNode::UHallNode()
{
	
}

float UHallNode::GetHalfDistanceOfRoom(EMazeOrientation Orientation)
{
	return TILE_SCALE * HallLength * RoomTile * 0.5;
}

void UHallNode::SpawnMazeObject(UWorld* World, FVector Position, EMazeDirection Direction)
{
	if (!HallLevelInstanceRef)
	{
		return;
	}
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("HallNode->SpawnMazeObject: there is no World!"));
		return;
	}
	FVector TileSpawnLocation = Position;
	switch (Direction)
	{
	case EMazeDirection::Up:
		SpawnLocation = Position - FVector(0,GetHalfDistanceOfRoom(EMazeOrientation::Vertical),0);
		TileSpawnLocation = Position + FVector(0,RoomTile * TILE_SCALE * 0.5,0);
		break;
	case EMazeDirection::Down:
		SpawnLocation = Position + FVector(0,GetHalfDistanceOfRoom(EMazeOrientation::Vertical),0);
		TileSpawnLocation = Position - FVector(0,RoomTile * TILE_SCALE * 0.5,0);
		break;
	case EMazeDirection::Left:
		SpawnLocation = Position - FVector(GetHalfDistanceOfRoom(EMazeOrientation::Horizontal),0,0);
		TileSpawnLocation = Position + FVector(RoomTile * TILE_SCALE * 0.5,0,0);
		break;
	case EMazeDirection::Right:
		SpawnLocation = Position + FVector(GetHalfDistanceOfRoom(EMazeOrientation::Horizontal),0,0);
		TileSpawnLocation = Position - FVector(RoomTile * TILE_SCALE * 0.5,0,0);
		break;
	default:
		break;
	}
	
	FRotator Rotator = FRotator::ZeroRotator;
	Rotator.Yaw = RoomRotation;

	for (int i = 0; i < HallLength; i ++)
	{
		
		switch (Direction)
		{
			case EMazeDirection::Up:
				TileSpawnLocation -= FVector(0,RoomTile * TILE_SCALE,0);
				break;
			case EMazeDirection::Down:
				TileSpawnLocation += FVector(0,RoomTile * TILE_SCALE,0);
				break;
			case EMazeDirection::Left:
				TileSpawnLocation -= FVector(RoomTile * TILE_SCALE,0,0);
				break;
			case EMazeDirection::Right:
			TileSpawnLocation += FVector(RoomTile * TILE_SCALE,0,0);
				break;
			default:
				break;
		}
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AActor* MazeObject = World->SpawnActor<AActor>(
			HallLevelInstanceRef,
			TileSpawnLocation,
			Rotator,
			SpawnParams
		);
		if (MazeObject)
		{
			AMazeTileLevelInstance* SpawnedLevelInstance = Cast<AMazeTileLevelInstance>(MazeObject);
			SpawnedLevelInstance->SetNodeData(this);
			UE_LOG(LogTemp, Log, TEXT("%s Spawned successful."), *MazeObject->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Hall Node Spawned failed."));
		}
	}
}

