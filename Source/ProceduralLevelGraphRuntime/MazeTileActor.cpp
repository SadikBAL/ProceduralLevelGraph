#include "MazeTileActor.h"

AMazeTileActor::AMazeTileActor()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;
}

void AMazeTileActor::SetNodeData(UMazeNodeBase* BaseNode)
{
	//NodeData = BaseNode;
	if (BaseNode != nullptr)
	{
		if (BaseNode->UpNode)
		{
			TArray<FName> SearchedTags = GetMeshPartNames(EMazeDirection::Up,BaseNode->RoomRotation);
			SearchedTags.Add(FName("Door"));
			UpdateMeshPartVisibilities(SearchedTags, true);
		}
		else
		{
			TArray<FName> SearchedTags = GetMeshPartNames(EMazeDirection::Up,BaseNode->RoomRotation);
			SearchedTags.Add(FName("Wall"));
			UpdateMeshPartVisibilities(SearchedTags, true);
		}
		if (BaseNode->DownNode)
		{
			TArray<FName> SearchedTags = GetMeshPartNames(EMazeDirection::Down,BaseNode->RoomRotation);
			SearchedTags.Add(FName("Door"));
			UpdateMeshPartVisibilities(SearchedTags, true);
		}
		else
		{
			TArray<FName> SearchedTags = GetMeshPartNames(EMazeDirection::Down,BaseNode->RoomRotation);
			SearchedTags.Add(FName("Wall"));
			UpdateMeshPartVisibilities(SearchedTags, true);
		}
		if (BaseNode->LeftNode)
		{
			TArray<FName> SearchedTags = GetMeshPartNames(EMazeDirection::Left,BaseNode->RoomRotation);
			SearchedTags.Add(FName("Door"));
			UpdateMeshPartVisibilities(SearchedTags, true);
		}
		else
		{
			TArray<FName> SearchedTags = GetMeshPartNames(EMazeDirection::Left,BaseNode->RoomRotation);
			SearchedTags.Add(FName("Wall"));
			UpdateMeshPartVisibilities(SearchedTags, true);
		}
		if (BaseNode->RightNode)
		{
			TArray<FName> SearchedTags = GetMeshPartNames(EMazeDirection::Right,BaseNode->RoomRotation);
			SearchedTags.Add(FName("Door"));
			UpdateMeshPartVisibilities(SearchedTags, true);
		}
		else
		{
			TArray<FName> SearchedTags = GetMeshPartNames(EMazeDirection::Right,BaseNode->RoomRotation);
			SearchedTags.Add(FName("Wall"));
			UpdateMeshPartVisibilities(SearchedTags, true);
		}
	}
}

void AMazeTileActor::UpdateMeshPartVisibilities(TArray<FName> SearchedTags, bool bVisibility)
{
	TArray<USceneComponent*> ChildComponents;
	if (GetRootComponent())
	{
		GetRootComponent()->GetChildrenComponents(true, ChildComponents);
	}
	for (USceneComponent* Child : ChildComponents)
	{
		if (UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(Child))
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
			}
		}
	}
}

TArray<FName> AMazeTileActor::GetMeshPartNames(EMazeDirection LocalDirection, int LocalRotation)
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
	const EMazeDirection* CurrentDirection = DirectionRotationMap.FindKey(CurrentRotationIndex);
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
			}
		}
	}

	return AddTags;
}
