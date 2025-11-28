#include "RoomNode.h"

#include "ProceduralLevelGraphRuntime/ProceduralLevelGraphTypes.h"

URoomNode::URoomNode()
{
	
}

float URoomNode::GetHalfDistanceOfRoom(EMazeOrientation Orientation)
{
	if (RoomRotation == 90 || RoomRotation == 270)
	{
		if (Orientation == EMazeOrientation::Vertical)
		{
			return RoomWidth * TILE_SCALE * 0.5;
		}
		else
		{
			return RoomHeight * TILE_SCALE * 0.5;
		}
	}
	else
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


}

void URoomNode::SpawnMazeObject(UWorld* World, FVector Position, EMazeDirection Direction)
{
	if (RoomLevelInstanceRefs.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnMazeObject: RoomBlueprints array empty."));
		return;
	}
	int RandomIndex = FMath::RandRange(0, RoomLevelInstanceRefs.Num() - 1);
	TSubclassOf<AMazeTileLevelInstance> RandomSpawnClass = RoomLevelInstanceRefs[RandomIndex];
	const AMazeTileLevelInstance* RandomTile = GetDefault<AMazeTileLevelInstance>(RandomSpawnClass);
	if (!RandomSpawnClass)
	{
		return;
	}
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnMazeObject: there is no world."));
		return;
	}
	switch (Direction)
	{
	case EMazeDirection::Up:
		SpawnLocation = Position - FVector(0,GetHalfDistanceOfRoom(EMazeOrientation::Vertical),0);
		break;
	case EMazeDirection::Down:
		SpawnLocation = Position + FVector(0,GetHalfDistanceOfRoom(EMazeOrientation::Vertical),0);
		break;
	case EMazeDirection::Left:
		SpawnLocation = Position - FVector(GetHalfDistanceOfRoom(EMazeOrientation::Horizontal),0,0);
		break;
	case EMazeDirection::Right:
		SpawnLocation = Position + FVector(GetHalfDistanceOfRoom(EMazeOrientation::Horizontal),0,0);
		break;
	default:
		break;
	}
	
	FRotator RoomRotator = FRotator::ZeroRotator;
	RoomRotator.Yaw = RoomRotation;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* MazeObject = World->SpawnActor<AActor>(
		RandomSpawnClass,
		SpawnLocation,
		RoomRotator,
		SpawnParams
	);
	if (MazeObject)
	{
		AMazeTileLevelInstance* SpawnedLevelInstance = Cast<AMazeTileLevelInstance>(MazeObject);
		SpawnedLevelInstance->SetNodeData(this);
	}
	//MazeObject Spawned and Has Hall Blueprints.
	if (MazeObject && HallLevelInstanceRefs.Num() > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("%s Spawned successful"), *MazeObject->GetName());
		float LocalDeltaWidth = (RoomWidth - RandomTile->Width) * 0.5;
		float LocalDeltaHeight = (RoomHeight - RandomTile->Height) * 0.5;
		int RandomPartIndex = FMath::RandRange(0, HallLevelInstanceRefs.Num() - 1);
		TSubclassOf<AMazeTileLevelInstance> RandomPartSpawnClass = HallLevelInstanceRefs[RandomPartIndex];
		const AMazeTileLevelInstance* RandomPartTile = GetDefault<AMazeTileLevelInstance>(RandomPartSpawnClass);
		if (LocalDeltaHeight > 0)
		{
			int HallPartCount = LocalDeltaHeight;
			if (UpNode && HallPartCount > 0)
			{
				for (int i = 0; i < HallPartCount; ++i)
				{
					FRotator Rotator = FRotator::ZeroRotator;
					Rotator.Yaw = 0;
					AActor* TempActor = World->SpawnActor<AActor>(
						RandomPartSpawnClass,
						GetEdgePosition(EMazeDirection::Up) + FVector(0,(TILE_SCALE * 0.5) + (TILE_SCALE * i),0),
						Rotator,
						SpawnParams
					);
					AMazeTileLevelInstance* SpawnedLevelInstance = Cast<AMazeTileLevelInstance>(TempActor);
					SpawnedLevelInstance->SetNodeData(this);
				}
			}
			if (DownNode && HallPartCount > 0)
			{
				for (int i = 0; i < HallPartCount; ++i)
				{
					FRotator Rotator = FRotator::ZeroRotator;
					Rotator.Yaw = 0;
					AActor* TempActor = World->SpawnActor<AActor>(
						RandomPartSpawnClass,
						GetEdgePosition(EMazeDirection::Down) - FVector(0,(TILE_SCALE * 0.5) + (TILE_SCALE * i),0),
						Rotator,
						SpawnParams
					);
					AMazeTileLevelInstance* SpawnedLevelInstance = Cast<AMazeTileLevelInstance>(TempActor);
					SpawnedLevelInstance->SetNodeData(this);
				}
			}
		}
		if (LocalDeltaWidth > 0)
		{
			int HallPartCount = LocalDeltaWidth;
			if (LeftNode && HallPartCount > 0)
			{
				for (int i = 0; i < HallPartCount; ++i)
				{
					FRotator Rotator = FRotator::ZeroRotator;
					Rotator.Yaw = 90;
					AActor* TempActor = World->SpawnActor<AActor>(
						RandomPartSpawnClass,
						GetEdgePosition(EMazeDirection::Left) + FVector((TILE_SCALE * 0.5) + (TILE_SCALE * i),0,0),
						Rotator,
						SpawnParams
					);
					AMazeTileLevelInstance* SpawnedLevelInstance = Cast<AMazeTileLevelInstance>(TempActor);
					SpawnedLevelInstance->SetNodeData(this);
				}
			}
			if (RightNode && HallPartCount > 0)
			{
				for (int i = 0; i < HallPartCount; ++i)
				{
					FRotator Rotator = FRotator::ZeroRotator;
					Rotator.Yaw = 90;
					AActor* TempActor = World->SpawnActor<AActor>(
						RandomPartSpawnClass,
						GetEdgePosition(EMazeDirection::Right) - FVector((TILE_SCALE * 0.5) + (TILE_SCALE * i),0,0),
						Rotator,
						SpawnParams
					);
					AMazeTileLevelInstance* SpawnedLevelInstance = Cast<AMazeTileLevelInstance>(TempActor);
					SpawnedLevelInstance->SetNodeData(this);
				}
			}
		}
	}
}