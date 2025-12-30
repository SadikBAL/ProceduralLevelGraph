#include "PassagePoint.h"

APassagePoint::APassagePoint()
{
}

#if WITH_EDITOR

void APassagePoint::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	const FName PropertyName = (PropertyChangedEvent.Property != nullptr) 
					   ? PropertyChangedEvent.Property->GetFName() 
					   : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(APassagePoint, Floor))
	{
		FVector ActorLocation = GetActorLocation();
		ActorLocation.Z = GetFloorHeight(Floor);
		SetActorLocation(ActorLocation);
	}
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

#endif

void APassagePoint::BeginPlay()
{
	Super::BeginPlay();
}
