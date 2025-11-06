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
	int RandomIndex = FMath::RandRange(0, TileBlueprints.Num() - 1);
	TSubclassOf<AMazeTileActor> RandomSpawnClass = TileBlueprints[RandomIndex];
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
