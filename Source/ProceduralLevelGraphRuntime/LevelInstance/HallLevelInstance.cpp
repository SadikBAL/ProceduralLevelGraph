#include "HallLevelInstance.h"

AHallLevelInstance::AHallLevelInstance()
{
	
}
#if WITH_EDITOR
void AHallLevelInstance::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	OnHallDataChanged.Broadcast();
}
#endif
