#include "HallNode.h"

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

UHallNode::~UHallNode()
{
	
}

float UHallNode::GetHalfDistanceOfRoom(EMazeOrientation Orientation)
{
	return 400 * HallLength * 0.5;
}

AActor* UHallNode::SpawnMazeObject(UWorld* World, FVector Position)
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

	FRotator Rotator = FRotator::ZeroRotator;
	Rotator.Yaw = RoomRotation;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	MazeObject = World->SpawnActor<AActor>(
		ActorToSpawnClass,
		Position,
		Rotator,
		SpawnParams
	);
	MazeObject->SetActorScale3D(FVector(1 * HallLength,1,1));

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

