#include "PassagePoint.h"

#include "EngineUtils.h"
#include "Components/LightComponent.h"
#include "Components/PointLightComponent.h"

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
				ActorLocation.X = LevelBound->Width * 0.5 * -1 * 100;
				ActorLocation.Y = Offset * 100;
				ActorRotation.Yaw = 90.0f;
				break;
			case EMazeDirection::Right:
				ActorLocation.X = LevelBound->Width * 0.5 * 100;
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
	if (PropertyName == GET_MEMBER_NAME_CHECKED(APassagePoint, CurrentPassageStatus))
	{
		UpdatePassageStatus(CurrentPassageStatus);
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

void APassagePoint::UpdatePassageStatus(EPassageType Status) const
{
	for (auto Element : PassageActorMap)
	{
		if (Status == Element.Key)
		{
			for (AActor* ActorTemp : Element.Value.Actors)
			{
				if (!ActorTemp)
					continue;
				ActorTemp->SetActorHiddenInGame(false);
				TInlineComponentArray<UStaticMeshComponent*> MeshComponents;
				ActorTemp->GetComponents(MeshComponents);
				for (UStaticMeshComponent* MeshComp : MeshComponents)
				{
					MeshComp->SetVisibility(true);
					if (Element.Key == EPassageType::Door 
						|| Element.Key == EPassageType::Door_Double 
						|| Element.Key == EPassageType::Door_Vertical)
					{
						MeshComp->SetCollisionProfileName(FName("Interactable_BlockDynamic"));
					}
					else
					{
						MeshComp->SetCollisionProfileName(FName("QueryAndPhysics"));
					}
				}
				TInlineComponentArray<UPointLightComponent*> LightComponents;
				ActorTemp->GetComponents(LightComponents);
				for (UPointLightComponent* LightComp : LightComponents)
				{
					LightComp->SetVisibility(true);
				}
			}
		}
		else
		{
			for (AActor* ActorTemp : Element.Value.Actors)
			{
				if (!ActorTemp)
					continue;
				ActorTemp->SetActorHiddenInGame(true);
				TInlineComponentArray<UStaticMeshComponent*> MeshComponents;
				ActorTemp->GetComponents(MeshComponents);
				for (UStaticMeshComponent* MeshComp : MeshComponents)
				{
					MeshComp->SetVisibility(false);
					MeshComp->SetCollisionProfileName(FName("NoCollision"));
				}
				TInlineComponentArray<UPointLightComponent*> LightComponents;
				ActorTemp->GetComponents(LightComponents);
				for (UPointLightComponent* LightComp : LightComponents)
				{
					LightComp->SetVisibility(false);
				}
			}
		}
	}
}

bool APassagePoint::IsPassageDataMatchDoorData(FDoorData Data, bool bHallDoor)
{
	//Hall Calculation Hall has only 2 door. Up and Down
	if (bHallDoor)
	{
		if (Data.DoorLocation == DoorLocation)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	//Normal Room Calculation
	if (Data.DoorLocation != DoorLocation)
	{
		return false;
	}
	if (Data.DoorFloor != DoorFloor)
	{
		return false;
	}
	if (Data.Offset != Offset)
	{
		return false;
	}
	return true;
}




