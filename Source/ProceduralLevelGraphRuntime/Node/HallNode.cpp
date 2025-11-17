#include "HallNode.h"

#include "ProceduralLevelGraphRuntime/MazeTileActor.h"

UHallNode::UHallNode()
{
	static ConstructorHelpers::FClassFinder<AActor> BP_Room_ClassFinder(
	TEXT("Blueprint'/Game/LevelPrototyping/BP_Hall.BP_Hall_C'")
);
	if (BP_Room_ClassFinder.Succeeded())
	{
		ActorToSpawnClass = BP_Room_ClassFinder.Class;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("URoomNode constructor: BP_Room_C sınıfı bulunamadı!"));
	}
}

float UHallNode::GetHalfDistanceOfRoom(EMazeOrientation Orientation)
{
	return 400 * HallLength * 0.5;
}

void UHallNode::SpawnMazeObject(UWorld* World, FVector Position, EMazeDirection Direction)
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
	FVector TileSpawnLocation = Position;
	switch (Direction)
	{
	case EMazeDirection::Up:
		SpawnLocation = Position - FVector(0,GetHalfDistanceOfRoom(EMazeOrientation::Vertical),0);
		TileSpawnLocation = Position + FVector(0,TileScale * 0.5,0);
		break;
	case EMazeDirection::Down:
		SpawnLocation = Position + FVector(0,GetHalfDistanceOfRoom(EMazeOrientation::Vertical),0);
		TileSpawnLocation = Position - FVector(0,TileScale * 0.5,0);
		break;
	case EMazeDirection::Left:
		SpawnLocation = Position - FVector(GetHalfDistanceOfRoom(EMazeOrientation::Horizontal),0,0);
		TileSpawnLocation = Position + FVector(TileScale * 0.5,0,0);
		break;
	case EMazeDirection::Right:
		SpawnLocation = Position + FVector(GetHalfDistanceOfRoom(EMazeOrientation::Horizontal),0,0);
		TileSpawnLocation = Position - FVector(TileScale * 0.5,0,0);
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
				TileSpawnLocation -= FVector(0,TileScale,0);
				break;
			case EMazeDirection::Down:
				TileSpawnLocation += FVector(0,TileScale,0);
				break;
			case EMazeDirection::Left:
				TileSpawnLocation -= FVector(TileScale,0,0);
				break;
			case EMazeDirection::Right:
			TileSpawnLocation += FVector(TileScale,0,0);
				break;
			default:
				break;
		}
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AActor* MazeObject = World->SpawnActor<AActor>(
			ActorToSpawnClass,
			TileSpawnLocation,
			Rotator,
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
	

	
}

