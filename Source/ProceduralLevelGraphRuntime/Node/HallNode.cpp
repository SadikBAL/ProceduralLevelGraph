#include "HallNode.h"

#include "ProceduralLevelGraphRuntime/ProceduralLevelGraphTypes.h"

UHallNode::UHallNode()
{
	
}

float UHallNode::GetHalfDistanceOfRoom(EMazeOrientation Orientation)
{
	return TILE_SCALE * HallLength * 0.5;
}

void UHallNode::SpawnMazeObject(UWorld* World, FVector Position, EMazeDirection Direction)
{
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("HallNode->SpawnMazeObject: there is no World!"));
		return;
	}
	if (!HallData)
	{
		UE_LOG(LogTemp, Warning, TEXT("HallNode->SpawnMazeObject: there is no HallData!"));
		return;
	}
	int HallStartTileHeight = 0;
	int HallEndTileHeight  = 0;
	if (HallData->HallStartTile)
	{
		HallStartTileHeight = HallData->HallStartTile->GetDefaultObject<AHallLevelInstance>()->Height;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HallNode->SpawnMazeObject: there is no HallStartTile!"));
	}
	if (HallData->HallEndTile)
	{
		HallEndTileHeight	= HallData->HallEndTile->GetDefaultObject<AHallLevelInstance>()->Height;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HallNode->SpawnMazeObject: there is no HallEndTileHeight!"));
	}
	
	FVector TileSpawnLocation = Position;
	switch (Direction)
	{
		case EMazeDirection::Up:
			SpawnLocation = Position - FVector(0,GetHalfDistanceOfRoom(EMazeOrientation::Vertical),0);
			TileSpawnLocation = Position;
			break;
		case EMazeDirection::Down:
			SpawnLocation = Position + FVector(0,GetHalfDistanceOfRoom(EMazeOrientation::Vertical),0);
			TileSpawnLocation = Position;
			break;
		case EMazeDirection::Left:
			SpawnLocation = Position - FVector(GetHalfDistanceOfRoom(EMazeOrientation::Horizontal),0,0);
			TileSpawnLocation = Position;
			break;
		case EMazeDirection::Right:
			SpawnLocation = Position + FVector(GetHalfDistanceOfRoom(EMazeOrientation::Horizontal),0,0);
			TileSpawnLocation = Position;
			break;
		default:
		break;
	}
	FRotator Rotator = FRotator::ZeroRotator;
	Rotator.Yaw = RoomRotation;
	
	if (HallData->HallStartTile)
	{
		FRotator HallTileRotator = FRotator::ZeroRotator;
		switch (Direction)
		{
			case EMazeDirection::Up:
				TileSpawnLocation -= FVector(0,HallStartTileHeight * TILE_SCALE * 0.5,0);
				HallTileRotator.Yaw = 0;
				break;
			case EMazeDirection::Down:
				TileSpawnLocation += FVector(0,HallStartTileHeight * TILE_SCALE * 0.5,0);
				HallTileRotator.Yaw = 180;
				break;
			case EMazeDirection::Left:
				TileSpawnLocation -= FVector(HallStartTileHeight * TILE_SCALE * 0.5,0,0);
				HallTileRotator.Yaw = 270;
				break;
			case EMazeDirection::Right:
				TileSpawnLocation += FVector(HallStartTileHeight * TILE_SCALE * 0.5,0,0);
				HallTileRotator.Yaw = 90;
				break;
			default:
				break;
		}
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AActor* MazeObject = World->SpawnActor<AActor>(
			HallData->HallStartTile,
			TileSpawnLocation,
			HallTileRotator,
			SpawnParams
		);
		if (MazeObject)
		{
			AMazeTileLevelInstance* SpawnedLevelInstance = Cast<AMazeTileLevelInstance>(MazeObject);
			SpawnedLevelInstance->SetNodeData(this);
			UE_LOG(LogTemp, Log, TEXT("HallData->HallStartTile %s Spawned successful."), *MazeObject->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("HallData->HallStartTile Node Spawned failed."));
		}
		switch (Direction)
		{
		case EMazeDirection::Up:
			TileSpawnLocation -= FVector(0,HallStartTileHeight * TILE_SCALE * 0.5,0);
			break;
		case EMazeDirection::Down:
			TileSpawnLocation += FVector(0,HallStartTileHeight * TILE_SCALE * 0.5,0);
			break;
		case EMazeDirection::Left:
			TileSpawnLocation -= FVector(HallStartTileHeight * TILE_SCALE * 0.5,0,0);
			break;
		case EMazeDirection::Right:
			TileSpawnLocation += FVector(HallStartTileHeight * TILE_SCALE * 0.5,0,0);
			break;
		default:
			break;
		}
	}
	if (HallData->HallTiles.Num() != 0)
	{
		int RetryCounter = 99;
		int NeededHeight = (HallLength - (HallStartTileHeight + HallEndTileHeight));
		while (NeededHeight > 0 && RetryCounter > 0 && HallData->HallTiles.Num() != 0)
		{
			int RandomIndex = FMath::RandRange(0, HallData->HallTiles.Num() - 1);
			int RandomTileHeight = HallData->HallTiles[RandomIndex]->GetDefaultObject<AHallLevelInstance>()->Height;
			if (NeededHeight - RandomTileHeight >= 0)
			{
				NeededHeight -= RandomTileHeight;
				switch (Direction)
				{
				case EMazeDirection::Up:
					TileSpawnLocation -= FVector(0,RandomTileHeight * TILE_SCALE * 0.5,0);
					break;
				case EMazeDirection::Down:
					TileSpawnLocation += FVector(0,RandomTileHeight * TILE_SCALE * 0.5,0);
					break;
				case EMazeDirection::Left:
					TileSpawnLocation -= FVector(RandomTileHeight * TILE_SCALE * 0.5,0,0);
					break;
				case EMazeDirection::Right:
					TileSpawnLocation += FVector(RandomTileHeight * TILE_SCALE * 0.5,0,0);
					break;
				default:
					break;
				}
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				AActor* MazeObject = World->SpawnActor<AActor>(
					HallData->HallTiles[RandomIndex],
					TileSpawnLocation,
					Rotator,
					SpawnParams
				);
				if (MazeObject)
				{
					AMazeTileLevelInstance* SpawnedLevelInstance = Cast<AMazeTileLevelInstance>(MazeObject);
					SpawnedLevelInstance->SetNodeData(this);
					UE_LOG(LogTemp, Log, TEXT("HallData->HallTiles %s Spawned successful."), *MazeObject->GetName());
				}
				else
				{
					UE_LOG(LogTemp, Log, TEXT("HallData->HallTiles Node Spawned failed."));
				}
				switch (Direction)
				{
				case EMazeDirection::Up:
					TileSpawnLocation -= FVector(0,RandomTileHeight * TILE_SCALE * 0.5,0);
					break;
				case EMazeDirection::Down:
					TileSpawnLocation += FVector(0,RandomTileHeight * TILE_SCALE * 0.5,0);
					break;
				case EMazeDirection::Left:
					TileSpawnLocation -= FVector(RandomTileHeight * TILE_SCALE * 0.5,0,0);
					break;
				case EMazeDirection::Right:
					TileSpawnLocation += FVector(RandomTileHeight * TILE_SCALE * 0.5,0,0);
					break;
				default:
					break;
				}
			}
			else
			{
				RetryCounter--;
			}
		}
		if (RetryCounter <= 0)
		{
			UE_LOG(LogTemp, Error, TEXT("HallData->HallTiles.Num() Retry Error!"));
		}

	}
	if (HallData->HallEndTile)
	{
		FRotator HallTileRotator = FRotator::ZeroRotator;
		switch (Direction)
		{
		case EMazeDirection::Up:
			TileSpawnLocation -= FVector(0,HallEndTileHeight * TILE_SCALE * 0.5,0);
			HallTileRotator.Yaw = 180;
			break;
		case EMazeDirection::Down:
			TileSpawnLocation += FVector(0,HallEndTileHeight * TILE_SCALE * 0.5,0);
			HallTileRotator.Yaw = 0;
			break;
		case EMazeDirection::Left:
			TileSpawnLocation -= FVector(HallEndTileHeight * TILE_SCALE * 0.5,0,0);
			HallTileRotator.Yaw = 90;
			break;
		case EMazeDirection::Right:
			TileSpawnLocation += FVector(HallEndTileHeight * TILE_SCALE * 0.5,0,0);
			HallTileRotator.Yaw = 270;
			break;
		default:
			break;
		}
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AActor* MazeObject = World->SpawnActor<AActor>(
			HallData->HallEndTile,
			TileSpawnLocation,
			HallTileRotator,
			SpawnParams
		);
		if (MazeObject)
		{
			AMazeTileLevelInstance* SpawnedLevelInstance = Cast<AMazeTileLevelInstance>(MazeObject);
			SpawnedLevelInstance->SetNodeData(this);
			UE_LOG(LogTemp, Log, TEXT("HallData->HallEndTile %s Spawned successful."), *MazeObject->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("HallData->HallEndTile Node Spawned failed."));
		}
	}
}