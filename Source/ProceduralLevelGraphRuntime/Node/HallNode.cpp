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
	if (HallData->HallUpCap)
	{
		HallStartTileHeight = HallData->HallUpCap->GetDefaultObject<AHallLevelInstance>()->Height;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HallNode->SpawnMazeObject: there is no HallUpCap!"));
	}
	if (HallData->HallDownCap)
	{
		HallEndTileHeight	= HallData->HallDownCap->GetDefaultObject<AHallLevelInstance>()->Height;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HallNode->SpawnMazeObject: there is no HallDownCap!"));
	}
	
	FRotator RoomRotator = FRotator::ZeroRotator;
	RoomRotator.Yaw = RoomRotation;
	
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

	TSubclassOf<AHallLevelInstance> Start = nullptr;
	TSubclassOf<AHallLevelInstance> End = nullptr;
	bool bResult = false;
	if (Direction == EMazeDirection::Up || Direction == EMazeDirection::Right)
	{
		Start = HallData->HallUpCap;
		End = HallData->HallDownCap;
		bResult = true;
	}
	else
	{
		Start = HallData->HallDownCap;
		End = HallData->HallUpCap;
	}
	if (RoomRotation == 90 || RoomRotation == 0)
		bResult = !bResult;
	if (Start)
	{
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
		TileSpawnLocation.Z = GetFloorHeight(RoomFloor);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AActor* MazeObject = World->SpawnActor<AActor>(
			Start,
			TileSpawnLocation,
			RoomRotator,
			SpawnParams
		);
		if (MazeObject)
		{
			AMazeTileLevelInstance* SpawnedLevelInstance = Cast<AMazeTileLevelInstance>(MazeObject);
			if (bResult)
				SpawnedLevelInstance->SetNodeDataFromHall(this,EMazeDirection::Up);
			else
				SpawnedLevelInstance->SetNodeDataFromHall(this,EMazeDirection::Down);
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
			if (HallData->HallTiles[RandomIndex] == nullptr)
				continue;
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
				TileSpawnLocation.Z = GetFloorHeight(RoomFloor);
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				AActor* MazeObject = World->SpawnActor<AActor>(
					HallData->HallTiles[RandomIndex],
					TileSpawnLocation,
					RoomRotator,
					SpawnParams
				);
				if (MazeObject)
				{
					AMazeTileLevelInstance* SpawnedLevelInstance = Cast<AMazeTileLevelInstance>(MazeObject);
					SpawnedLevelInstance->SetNodeDataFromHall(this,EMazeDirection::None);
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
	if (End)
	{
		switch (Direction)
		{
		case EMazeDirection::Up:
			TileSpawnLocation -= FVector(0,HallEndTileHeight * TILE_SCALE * 0.5,0);
			break;
		case EMazeDirection::Down:
			TileSpawnLocation += FVector(0,HallEndTileHeight * TILE_SCALE * 0.5,0);
			break;
		case EMazeDirection::Left:
			TileSpawnLocation -= FVector(HallEndTileHeight * TILE_SCALE * 0.5,0,0);
			break;
		case EMazeDirection::Right:
			TileSpawnLocation += FVector(HallEndTileHeight * TILE_SCALE * 0.5,0,0);
			break;
		default:
			break;
		}
		TileSpawnLocation.Z = GetFloorHeight(RoomFloor);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AActor* MazeObject = World->SpawnActor<AActor>(
			End,
			TileSpawnLocation,
			RoomRotator,
			SpawnParams
		);
		if (MazeObject)
		{
			AMazeTileLevelInstance* SpawnedLevelInstance = Cast<AMazeTileLevelInstance>(MazeObject);
			if (bResult)
				SpawnedLevelInstance->SetNodeDataFromHall(this,EMazeDirection::Down);
			else
				SpawnedLevelInstance->SetNodeDataFromHall(this,EMazeDirection::Up);
			UE_LOG(LogTemp, Log, TEXT("HallData->HallEndTile %s Spawned successful."), *MazeObject->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("HallData->HallEndTile Node Spawned failed."));
		}
	}
}