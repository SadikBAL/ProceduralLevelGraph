#include "RoomNode.h"

#include "ProceduralLevelGraphRuntime/MazeTileActor.h"

URoomNode::URoomNode()
{
	static ConstructorHelpers::FClassFinder<AActor> BP_Room_ClassFinder(
		TEXT("Blueprint'/Game/LevelPrototyping/BP_Room.BP_Room_C'")
		
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

float URoomNode::GetHalfDistanceOfRoom(EMazeOrientation Orientation)
{
	if (Orientation == EMazeOrientation::Horizontal)
	{
		return RoomWidth * 400.0 * 0.5;
	}
	else
	{
		return RoomHeight * 400.0 * 0.5;
	}

}

AActor* URoomNode::SpawnMazeObject(UWorld* World, FVector Position)
{
	int RandomIndex = FMath::RandRange(0, RoomBlueprints.Num() - 1);
	TSubclassOf<AMazeTileActor> RandomSpawnClass = RoomBlueprints[RandomIndex];
	const AMazeTileActor* RandomTile = GetDefault<AMazeTileActor>(RandomSpawnClass);
	if (!RandomSpawnClass)
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
		RandomSpawnClass,
		Position,
		FRotator::ZeroRotator,
		SpawnParams
	);
	//MazeObject Spawned and Has Hall Bluprints.
	if (MazeObject && HallBlueprints.Num() > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("%s Spawn edildi."), *MazeObject->GetName());
		float LocalDeltaWidth = (RoomWidth - RandomTile->Width) * 0.5;
		float LocalDeltaHeight = (RoomHeight - RandomTile->Height) * 0.5;
		int RandomPartIndex = FMath::RandRange(0, HallBlueprints.Num() - 1);
		TSubclassOf<AMazeTileActor> RandomPartSpawnClass = HallBlueprints[RandomPartIndex];
		const AMazeTileActor* RandomPartTile = GetDefault<AMazeTileActor>(RandomPartSpawnClass);
		if (LocalDeltaHeight > 0)
		{
			if (UpNode)
			{
				FRotator Rotator = FRotator::ZeroRotator;
				Rotator.Yaw = 0;
				AActor* TempActor = World->SpawnActor<AActor>(
					RandomPartSpawnClass,
					GetEdgePosition(EMazeDirection::Up) + FVector(0,LocalDeltaHeight * 400 * 0.5,0),
					Rotator,
					SpawnParams
				);
				TempActor->SetActorScale3D(FVector(1 * LocalDeltaHeight,1,1));
				PartObjects.Add(TempActor);
				
			}
			if (DownNode)
			{
				FRotator Rotator = FRotator::ZeroRotator;
				Rotator.Yaw = 0;
				AActor* TempActor = World->SpawnActor<AActor>(
					RandomPartSpawnClass,
					GetEdgePosition(EMazeDirection::Down) - FVector(0,LocalDeltaHeight * 400 * 0.5,0),
					Rotator,
					SpawnParams
				);
				TempActor->SetActorScale3D(FVector(1 * LocalDeltaHeight,1,1));
				PartObjects.Add(TempActor);
			}
		}
		if (LocalDeltaWidth > 0)
		{
			if (LeftNode)
			{
				FRotator Rotator = FRotator::ZeroRotator;
				Rotator.Yaw = 90;
				AActor* TempActor = World->SpawnActor<AActor>(
					RandomPartSpawnClass,
					GetEdgePosition(EMazeDirection::Left) + FVector(LocalDeltaWidth * 400 * 0.5,0,0),
					Rotator,
					SpawnParams
				);
				TempActor->SetActorScale3D(FVector(1 * LocalDeltaWidth,1,1));
				PartObjects.Add(TempActor);
			}
			if (RightNode)
			{
				FRotator Rotator = FRotator::ZeroRotator;
				Rotator.Yaw = 90;
				AActor* TempActor = World->SpawnActor<AActor>(
					RandomPartSpawnClass,
					GetEdgePosition(EMazeDirection::Right) - FVector(LocalDeltaWidth * 400 * 0.5,0,0),
					Rotator,
					SpawnParams
				);
				TempActor->SetActorScale3D(FVector(1 * LocalDeltaWidth,1,1));
				PartObjects.Add(TempActor);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s spawn edilemedi"), *ActorToSpawnClass->GetName());
	}
	return MazeObject.Get();
}

AActor* URoomNode::SpawnMazeExtraPart(UWorld* World, FVector Position)
{
	
	return nullptr;	
}
