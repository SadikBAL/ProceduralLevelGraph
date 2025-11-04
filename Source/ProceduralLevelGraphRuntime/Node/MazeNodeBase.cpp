#include "MazeNodeBase.h"

AActor* UMazeNodeBase::SpawnMazeObject(UWorld* World,FVector Position)
{
	if (!ActorToSpawnClass)
	{
		return nullptr;
	}

	
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnMyRoom: Geçerli bir dünya (World) bulunamadı!"));
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	MazeObject = World->SpawnActor<AActor>(
		ActorToSpawnClass,
		Position,
		FRotator::ZeroRotator,
		SpawnParams
	);
	

	if (MazeObject)
	{
		UE_LOG(LogTemp, Log, TEXT("%s Spawn edildi."), *MazeObject->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s spawn edilemedi"), *ActorToSpawnClass->GetName());
	}
	return MazeObject.Get();
}

FVector UMazeNodeBase::GetEdgePosition(EMazeDirection Direction)
{
	switch (Direction)
	{
	case EMazeDirection::Up:
		return MazeObject->GetActorLocation() + FVector(0, GetHalfDistanceOfRoom(EMazeOrientation::Vertical) * -1, 1);
	case EMazeDirection::Down:
		return MazeObject->GetActorLocation() + FVector(0, GetHalfDistanceOfRoom(EMazeOrientation::Vertical), 1);
	case EMazeDirection::Left:
		return MazeObject->GetActorLocation() + FVector(GetHalfDistanceOfRoom(EMazeOrientation::Horizontal) * -1, 0, 1);
	case EMazeDirection::Right:
		return MazeObject->GetActorLocation() + FVector(GetHalfDistanceOfRoom(EMazeOrientation::Horizontal), 0, 1);
	default:
		return MazeObject->GetActorLocation();
		break;
	}
}

float UMazeNodeBase::GetHalfDistanceOfRoom(EMazeOrientation Orientation)
{
	return 0.0f;
}
