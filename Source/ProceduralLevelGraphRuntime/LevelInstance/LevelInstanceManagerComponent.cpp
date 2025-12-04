#include "LevelInstanceManagerComponent.h"

ULevelInstanceManagerComponent::ULevelInstanceManagerComponent()
{
	
}

void ULevelInstanceManagerComponent::OnRegister()
{
	Super::OnRegister();
}

void ULevelInstanceManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	if (LevelInstance)
	{
		LevelInstance->ApplyMazeTileData();
	}
}
