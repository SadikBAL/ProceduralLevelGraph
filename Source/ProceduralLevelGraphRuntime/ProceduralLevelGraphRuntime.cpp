#include "ProceduralLevelGraphRuntime.h"

#if WITH_EDITOR
#include "EditorActorFolders.h"
#endif

#include "EngineUtils.h"
#include "NavigationSystem.h"
#include "ProceduralLevelGraphTypes.h"
#include "Components/BrushComponent.h"
#include "LevelInstance/MazeTileLevelInstance.h"
#include "NavMesh/NavMeshBoundsVolume.h"
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
	TSet<FFolder> FoldersToDelete;
	for (TActorIterator<AActor> It(World); It; ++It)
	{
		AActor* ActorRef = *It;
		if (ActorRef && ActorRef->IsA(AMazeTileLevelInstance::StaticClass()))
		{
			if (AMazeTileLevelInstance* Instance = Cast<AMazeTileLevelInstance>(ActorRef))
			{
				FFolder ActorFolder = ActorRef->GetFolder();
           
				if (ActorFolder.IsValid())
				{
					FoldersToDelete.Add(ActorFolder);
				}
				if (Instance->LevelStreamingDynamic)
				{
					if (ULevel* LoadedLevel = Instance->LevelStreamingDynamic->GetLoadedLevel())
					{
						TArray<AActor*> LevelActors = LoadedLevel->Actors; 
                
						for (AActor* ActorInLevel : LevelActors)
						{
							if (IsValid(ActorInLevel))
							{
								World->EditorDestroyActor(ActorInLevel, true);
							}
						}
					}
					Instance->LevelStreamingDynamic->SetShouldBeVisible(false);
					Instance->LevelStreamingDynamic->SetShouldBeLoaded(false);
					World->RemoveStreamingLevel(Instance->LevelStreamingDynamic);
					Instance->LevelStreamingDynamic = nullptr;
				}
			}
			ActorRef->Destroy();
		}
		else if (ActorRef && ActorRef->IsA(ANavMeshBoundsVolume::StaticClass()))
		{
			ActorRef->Destroy();
		}
	}
	if (FoldersToDelete.Num() > 0)
	{
		FActorFolders& ActorFolders = FActorFolders::Get();

		for (const FFolder& Folder : FoldersToDelete)
		{
			if (World)
			{
				ActorFolders.DeleteFolder(*World, Folder);
			}
		}
	}
}

void UProceduralLevelGraphRuntime::RecreateMaze()
{
	DeleteMaze();
	CreateMaze();
}

#endif

void UProceduralLevelGraphRuntime:: SpawnNode(UWorld* World, UMazeNodeBase* MazeNodeBase, EMazeDirection Direction, FVector Location)
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
		//Its Recursive and Create all Maze of Parts
		SpawnNode(GEngine->GetWorldFromContextObject(World, EGetWorldErrorMode::LogAndReturnNull),Entrance,EMazeDirection::None,Entrance->RoomPosition);
		//Spawn Navmesh
		if (World)
	    {
	        FActorSpawnParameters SpawnParams;
	        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	        if (ANavMeshBoundsVolume* NavVolume = World->SpawnActor<ANavMeshBoundsVolume>(ANavMeshBoundsVolume::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams))
	        {
	            const FVector MazeCenterLocation = FVector(0.f, 0.f, 0.f);
	            const FVector MazeExtent = FVector(10000.f, 50000.f, 1000.f);

	            NavVolume->SetActorLocation(MazeCenterLocation);
	            if (UBrushComponent* BrushComp = NavVolume->GetBrushComponent())
	            {
	                FVector MinBounds = MazeCenterLocation - MazeExtent;
	                FVector MaxBounds = MazeCenterLocation + MazeExtent;
	                BrushComp->Bounds = FBox(MinBounds, MaxBounds);
	            }
	            if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World))
	            {
	                NavSys->OnNavigationBoundsUpdated(NavVolume);
	                NavSys->Build();
	            }
	        }
	    }
	}
}
