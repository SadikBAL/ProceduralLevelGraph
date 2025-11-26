#include "MazeNodeBase.h"

#include "ProceduralLevelGraphRuntime/Actor/MazeTileActor.h"

void UMazeNodeBase::SpawnMazeObject(UWorld* World,FVector Position, EMazeDirection Direction)
{
	if (!ActorToSpawnClass)
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
		ActorToSpawnClass,
		SpawnLocation,
		FRotator::ZeroRotator,
		SpawnParams
	);

	if (MazeObject)
	{
		AMazeTileActor* SpawnedMazeTileActor = Cast<AMazeTileActor>(MazeObject);
		SpawnedMazeTileActor->SetNodeData(this);
		UE_LOG(LogTemp, Log, TEXT("%s Spawn edildi."), *MazeObject->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s spawn edilemedi"), *ActorToSpawnClass->GetName());
	}
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
