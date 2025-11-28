#include "MazeTileLevelInstance.h"

#include "Engine/StaticMeshActor.h"
#include "ProceduralLevelGraphRuntime/ProceduralLevelGraphTypes.h"

AMazeTileLevelInstance::AMazeTileLevelInstance()
{
	
}

void AMazeTileLevelInstance::OnLevelInstanceLoaded()
{
	if (NodeData != nullptr)
	{
		
		if (NodeData->UpNode)
		{
			TArray<FName> SearchedTags = GetMeshPartNames(EMazeDirection::Up,NodeData->RoomRotation);
			SearchedTags.Add(FName("Door"));
			UpdateMeshPartVisibilities(SearchedTags, true);

			SearchedTags = GetMeshPartNames(EMazeDirection::Up,NodeData->RoomRotation);
			SearchedTags.Add(FName("Wall"));
			UpdateMeshPartVisibilities(SearchedTags, false);
		}
		else
		{
			TArray<FName> SearchedTags = GetMeshPartNames(EMazeDirection::Up,NodeData->RoomRotation);
			SearchedTags.Add(FName("Wall"));
			UpdateMeshPartVisibilities(SearchedTags, true);
			
			SearchedTags = GetMeshPartNames(EMazeDirection::Up,NodeData->RoomRotation);
			SearchedTags.Add(FName("Door"));
			UpdateMeshPartVisibilities(SearchedTags, false);
		}
		if (NodeData->DownNode)
		{
			TArray<FName> SearchedTags = GetMeshPartNames(EMazeDirection::Down,NodeData->RoomRotation);
			SearchedTags.Add(FName("Door"));
			UpdateMeshPartVisibilities(SearchedTags, true);
			
			SearchedTags = GetMeshPartNames(EMazeDirection::Down,NodeData->RoomRotation);
			SearchedTags.Add(FName("Wall"));
			UpdateMeshPartVisibilities(SearchedTags, false);
			
		}
		else
		{
			TArray<FName> SearchedTags = GetMeshPartNames(EMazeDirection::Down,NodeData->RoomRotation);
			SearchedTags.Add(FName("Wall"));
			UpdateMeshPartVisibilities(SearchedTags, true);
			
			SearchedTags = GetMeshPartNames(EMazeDirection::Down,NodeData->RoomRotation);
			SearchedTags.Add(FName("Door"));
			UpdateMeshPartVisibilities(SearchedTags, false);
		}
		if (NodeData->LeftNode)
		{
			TArray<FName> SearchedTags = GetMeshPartNames(EMazeDirection::Left,NodeData->RoomRotation);
			SearchedTags.Add(FName("Door"));
			UpdateMeshPartVisibilities(SearchedTags, true);
			
			SearchedTags = GetMeshPartNames(EMazeDirection::Left,NodeData->RoomRotation);
			SearchedTags.Add(FName("Wall"));
			UpdateMeshPartVisibilities(SearchedTags, false);
		}
		else
		{
			TArray<FName> SearchedTags = GetMeshPartNames(EMazeDirection::Left,NodeData->RoomRotation);
			SearchedTags.Add(FName("Wall"));
			UpdateMeshPartVisibilities(SearchedTags, true);
			
			SearchedTags = GetMeshPartNames(EMazeDirection::Left,NodeData->RoomRotation);
			SearchedTags.Add(FName("Door"));
			UpdateMeshPartVisibilities(SearchedTags, false);
		}
		if (NodeData->RightNode)
		{
			TArray<FName> SearchedTags = GetMeshPartNames(EMazeDirection::Right,NodeData->RoomRotation);
			SearchedTags.Add(FName("Door"));
			UpdateMeshPartVisibilities(SearchedTags, true);
			
			SearchedTags = GetMeshPartNames(EMazeDirection::Right,NodeData->RoomRotation);
			SearchedTags.Add(FName("Wall"));
			UpdateMeshPartVisibilities(SearchedTags, false);
		}
		else
		{
			TArray<FName> SearchedTags = GetMeshPartNames(EMazeDirection::Right,NodeData->RoomRotation);
			SearchedTags.Add(FName("Wall"));
			UpdateMeshPartVisibilities(SearchedTags, true);
			
			SearchedTags = GetMeshPartNames(EMazeDirection::Right,NodeData->RoomRotation);
			SearchedTags.Add(FName("Door"));
			UpdateMeshPartVisibilities(SearchedTags, false);
		}
	}
	Super::OnLevelInstanceLoaded();
}

void AMazeTileLevelInstance::SetNodeData(UMazeNodeBase* BaseNode)
{
	NodeData = BaseNode;
}

void AMazeTileLevelInstance::UpdateMeshPartVisibilities(TArray<FName> SearchedTags, bool bVisibility)
{
	if (ULevel* LoadedLevel = GetLoadedLevel())
	{
		for (AActor* Actor : LoadedLevel->Actors)
		{
			TInlineComponentArray<UStaticMeshComponent*> MeshComponents;
			Actor->GetComponents(MeshComponents);
			for (UStaticMeshComponent* MeshComp : MeshComponents)
			{
				if (MeshComp)
				{
					bool bMatched = true;
					for (FName SearchTag : SearchedTags)
					{
						if (!MeshComp->ComponentHasTag(SearchTag))
						{
							bMatched = false;
							break;
						}
					}
					if (bMatched)
					{
						MeshComp->SetVisibility(bVisibility);
						if (!bVisibility)
						{
							MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
						}
						else
						{
							MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
						}
					}
					UE_LOG(LogTemp, Log, TEXT("Bulunan Mesh Component: %s (Actor: %s)"), 
						*MeshComp->GetName(), 
						*Actor->GetName());
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UpdateMeshPartVisibilities: Level not loaded."));
	}
}

TArray<FName> AMazeTileLevelInstance::GetMeshPartNames(EMazeDirection LocalDirection, int LocalRotation)
{
	TMap<EMazeDirection, int> DirectionRotationMap;
	DirectionRotationMap.Add(EMazeDirection::Up, 0);
	DirectionRotationMap.Add(EMazeDirection::Right, 1);
	DirectionRotationMap.Add(EMazeDirection::Down,2);
	DirectionRotationMap.Add(EMazeDirection::Left,3);
	int CurrentRotationIndex = DirectionRotationMap[LocalDirection];
	int Counter = LocalRotation / 90;
	CurrentRotationIndex += Counter;
	CurrentRotationIndex %= 4;
	//const EMazeDirection* CurrentDirection = DirectionRotationMap.FindKey(CurrentRotationIndex);
	TArray<FName> AddTags;
	for (auto Value : DirectionRotationMap)
	{
		if (Value.Value == CurrentRotationIndex)
		{
			switch (Value.Key)
			{
			case EMazeDirection::Up:
				AddTags.Add(FName("Up"));
				break;
			case EMazeDirection::Right:
				AddTags.Add(FName("Right"));
				break;
			case EMazeDirection::Down:
				AddTags.Add(FName("Down"));
				break;
			case EMazeDirection::Left:
				AddTags.Add(FName("Left"));
				break;
			default:
				break;
			}
		}
	}

	return AddTags;
}
