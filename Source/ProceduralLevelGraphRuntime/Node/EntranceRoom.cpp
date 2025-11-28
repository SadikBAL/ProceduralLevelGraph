#include "EntranceRoom.h"

#include "RoomNode.h"
#include "ProceduralLevelGraphRuntime/ProceduralLevelGraphTypes.h"

UEntranceNode::UEntranceNode()
{
}

float UEntranceNode::GetHalfDistanceOfRoom(EMazeOrientation Orientation)
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

void UEntranceNode::SpawnMazeObject(UWorld* World, FVector Position, EMazeDirection Direction)
{
	if (!EntranceInstanceRef)
	{
		return;
	}

	
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnMyRoom: Geçerli bir dünya (World) bulunamadı!"));
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
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* MazeObject = World->SpawnActor<AActor>(
		EntranceInstanceRef,
		SpawnLocation,
		FRotator::ZeroRotator,
		SpawnParams
	);

	if (MazeObject)
	{
		ARoomLevelInstance* SpawnedMazeTileActor = Cast<ARoomLevelInstance>(MazeObject);
		SpawnedMazeTileActor->SetNodeData(this);
		UE_LOG(LogTemp, Log, TEXT("%s Spawn successfully."), *MazeObject->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Entrance Room Node Spawned failed."));
	}
}

