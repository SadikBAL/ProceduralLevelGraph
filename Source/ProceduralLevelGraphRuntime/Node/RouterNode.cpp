#include "RouterNode.h"

#include "LevelInstance/LevelInstanceActor.h"
#include "ProceduralLevelGraphRuntime/ProceduralLevelGraphTypes.h"

URouterNode::URouterNode()
{
}

float URouterNode::GetHalfDistanceOfRoom(EMazeOrientation Orientation)
{
	return TILE_SCALE * 0.5;
}

void URouterNode::SpawnMazeObject(UWorld* World, FVector Position, EMazeDirection Direction)
{
	if (!RouterInstanceRef)
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
		RouterInstanceRef,
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
		UE_LOG(LogTemp, Log, TEXT("Router Node Spawned failed."));
	}
}
