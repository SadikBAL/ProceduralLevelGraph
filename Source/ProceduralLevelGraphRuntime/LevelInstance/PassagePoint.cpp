#include "PassagePoint.h"

#include "EngineUtils.h"

APassagePoint::APassagePoint()
{
}



#if WITH_EDITOR

void APassagePoint::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	const FName PropertyName = (PropertyChangedEvent.Property != nullptr) 
					   ? PropertyChangedEvent.Property->GetFName() 
					   : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(APassagePoint, DoorFloor))
	{
		FVector ActorLocation = GetActorLocation();
		ActorLocation.Z = GetFloorHeight(DoorFloor);
		SetActorLocation(ActorLocation);
	}
	if (	PropertyName == GET_MEMBER_NAME_CHECKED(APassagePoint, Offset) || 
			PropertyName == GET_MEMBER_NAME_CHECKED(APassagePoint, DoorLocation))
	{
		if (LevelBound == nullptr)
		{
			LevelBound = FindMyLevelBoundActor();
		}
		if (LevelBound)
		{
			FVector ActorLocation = GetActorLocation();
			FRotator ActorRotation = GetActorRotation();
			switch (DoorLocation)
			{
			case EMazeDirection::Down:
				ActorLocation.Y = LevelBound->Height * 0.5 * 100;
				ActorLocation.X = Offset * 100;
				ActorRotation.Yaw = 0.0f;
				break;
			case EMazeDirection::Left:
				ActorLocation.X = LevelBound->Width * 0.5 * 100;
				ActorLocation.Y = Offset * 100;
				ActorRotation.Yaw = 90.0f;
				break;
			case EMazeDirection::Right:
				ActorLocation.X = LevelBound->Width * 0.5 * -1 * 100;
				ActorLocation.Y = Offset * 100;
				ActorRotation.Yaw = 90.0f;
				break;
			case EMazeDirection::Up:
				ActorLocation.Y = LevelBound->Height * 0.5 * -1 * 100;
				ActorLocation.X = Offset * 100;
				ActorRotation.Yaw = 0.0f;
				break;
			default:
				break;
			}
			SetActorLocation(ActorLocation);
			SetActorRotation(ActorRotation);
		}
	}
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

ALevelBound* APassagePoint::FindMyLevelBoundActor()
{
	UWorld* World = GetWorld();
	if (!World) return nullptr;
	for (TActorIterator<ALevelBound> It(World); It; ++It)
	{
		ALevelBound* FoundActor = *It;
		if (IsValid(FoundActor))
		{
			return FoundActor;
		}
	}
	return nullptr;
}

#endif

void APassagePoint::BeginPlay()
{
	Super::BeginPlay();
}
