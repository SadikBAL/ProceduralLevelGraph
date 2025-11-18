#include "ProceduralLevelGraphRuntime.h"

#include "EngineUtils.h"
#include "MazeTileActor.h"
#include "Node/EntranceRoom.h"

UProceduralLevelGraphRuntime::UProceduralLevelGraphRuntime()
{
	
}



#if WITH_EDITOR
void UProceduralLevelGraphRuntime::SpawnMazeToEditor()
{
	if (UEntranceNode* Entrance = Cast<UEntranceNode>(StartNode))
	{
		Nodes.Empty();
		SpawnNode(GEditor->GetEditorWorldContext().World(),Entrance,EMazeDirection::None,Entrance->RoomPosition);
		Nodes.Empty();
	}
}


void UProceduralLevelGraphRuntime::CreateMaze()
{
	SpawnMazeToEditor();
}

void UProceduralLevelGraphRuntime::DeleteMaze()
{
	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		return;
	}
	for (TActorIterator<AActor> It(World); It; ++It)
	{
		AActor* Actor = *It;
		if (Actor && Actor->IsA(AMazeTileActor::StaticClass()))
		{
			Actor->Destroy();
		}
	}
}

void UProceduralLevelGraphRuntime::RecreateMaze()
{
	DeleteMaze();
	CreateMaze();
}

#endif

void UProceduralLevelGraphRuntime::SpawnNode(UWorld* World, UMazeNodeBase* MazeNodeBase, EMazeDirection Direction, FVector Location)
{
	if (Nodes.Find(MazeNodeBase) == INDEX_NONE)
	{
		Nodes.Add(MazeNodeBase);
		
		MazeNodeBase->SpawnMazeObject(World,Location,Direction);
		
		if (MazeNodeBase->UpNode)
		{
			SpawnNode(World,MazeNodeBase->UpNode,EMazeDirection::Up,MazeNodeBase->GetEdgePosition(EMazeDirection::Up));
		}
		if (MazeNodeBase->DownNode)
		{
			SpawnNode(World,MazeNodeBase->DownNode,EMazeDirection::Down,MazeNodeBase->GetEdgePosition(EMazeDirection::Down));
		}
		if (MazeNodeBase->LeftNode)
		{
			SpawnNode(World,MazeNodeBase->LeftNode,EMazeDirection::Left,MazeNodeBase->GetEdgePosition(EMazeDirection::Left));
		}
		if (MazeNodeBase->RightNode)
		{
			SpawnNode(World,MazeNodeBase->RightNode,EMazeDirection::Right,MazeNodeBase->GetEdgePosition(EMazeDirection::Right));
		}
	}
}


void UProceduralLevelGraphRuntime::SpawnMaze(UObject* WorldContextObject)
{
	
	if (UEntranceNode* Entrance = Cast<UEntranceNode>(StartNode))
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
		Nodes.Empty();
		SpawnNode(GEngine->GetWorldFromContextObject(World, EGetWorldErrorMode::LogAndReturnNull),Entrance,EMazeDirection::None,Entrance->RoomPosition);
	}
}