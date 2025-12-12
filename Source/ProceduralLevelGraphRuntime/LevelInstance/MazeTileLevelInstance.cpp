#include "MazeTileLevelInstance.h"

#include "LevelInstanceManagerComponent.h"
#include "Components/PointLightComponent.h"
#include "Engine/StaticMeshActor.h"
#include "ProceduralLevelGraphRuntime/ProceduralLevelGraphTypes.h"

#if WITH_EDITOR
#include "UObject/ObjectSaveContext.h"
#endif

AMazeTileLevelInstance::AMazeTileLevelInstance()
{
	LevelInstanceManager = CreateDefaultSubobject<ULevelInstanceManagerComponent>(TEXT("LevelInstanceManagerComponent"));
	LevelInstanceManager->LevelInstance = this;
	LevelInstanceManager->SetAutoActivate(true);
	SetActorTickEnabled(true);
}

void AMazeTileLevelInstance::OnLevelInstanceLoaded()
{
	Super::OnLevelInstanceLoaded();
	//ApplyMazeTileData();
}

bool AMazeTileLevelInstance::IsLoadingEnabled() const
{
	return false;
}

#if WITH_EDITOR
void AMazeTileLevelInstance::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);
	LevelName = WorldAsset.GetAssetName();
}
#endif
void AMazeTileLevelInstance::SetNodeData(UMazeNodeBase* BaseNode)
{
	NodeData.RoomRotation = BaseNode->RoomRotation;
	NodeData.MazeDirectionMap.Add(EMazeDirection::Up,	   (BaseNode->UpNode    ? EMazePinType::Tier1 : EMazePinType::Hidden));
	NodeData.MazeDirectionMap.Add(EMazeDirection::Down,  (BaseNode->DownNode  ? EMazePinType::Tier1 : EMazePinType::Hidden));
	NodeData.MazeDirectionMap.Add(EMazeDirection::Left,  (BaseNode->LeftNode  ? EMazePinType::Tier1 : EMazePinType::Hidden));
	NodeData.MazeDirectionMap.Add(EMazeDirection::Right, (BaseNode->RightNode ? EMazePinType::Tier1 : EMazePinType::Hidden));
	LoadLevelAsync();
}

void AMazeTileLevelInstance::UpdateMeshPartVisibilities(TArray<FName> SearchedTags, bool bVisibility)
{
	if (LevelStreamingDynamic)
	{
		if (ULevel* LoadedLevel = LevelStreamingDynamic->GetLoadedLevel())
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
								MeshComp->SetCollisionProfileName(FName("NoCollision"));
							}
							else
							{
								if (SearchedTags.Contains(FName("Door")))
								{
									MeshComp->SetCollisionProfileName(FName("Interactable_BlockDynamic"));
								}
								else
								{
									MeshComp->SetCollisionProfileName(FName("QueryAndPhysics"));
								}
								
							}
						}
						UE_LOG(LogTemp, Log, TEXT("Bulunan Mesh Component: %s (Actor: %s)"), 
							*MeshComp->GetName(), 
							*Actor->GetName());
					}
				}
				TInlineComponentArray<UPointLightComponent*> LightComponent;
				Actor->GetComponents(LightComponent);
				for (UPointLightComponent* PointComp : LightComponent)
				{
					if (PointComp)
					{
						bool bMatched = true;
						for (FName SearchTag : SearchedTags)
						{
							if (!PointComp->ComponentHasTag(SearchTag))
							{
								bMatched = false;
								break;
							}
						}
						if (bMatched)
						{
							PointComp->SetVisibility(bVisibility);
						}
						UE_LOG(LogTemp, Log, TEXT("Bulunan Mesh Component: %s (Actor: %s)"), 
							*PointComp->GetName(), 
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
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UpdateMeshPartVisibilities: LevelStreamingDynamic not loaded."));
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

void AMazeTileLevelInstance::ApplyMazeTileData()
{	
	if (NodeData.MazeDirectionMap.Contains(EMazeDirection::Up) && NodeData.MazeDirectionMap[EMazeDirection::Up] != EMazePinType::Hidden)
	{
		TArray<FName> SearchedTags = GetMeshPartNames(EMazeDirection::Up,NodeData.RoomRotation);
		SearchedTags.Add(FName("Door"));
		UpdateMeshPartVisibilities(SearchedTags, true);

		SearchedTags = GetMeshPartNames(EMazeDirection::Up,NodeData.RoomRotation);
		SearchedTags.Add(FName("Wall"));
		UpdateMeshPartVisibilities(SearchedTags, false);
	}
	else
	{
		TArray<FName> SearchedTags = GetMeshPartNames(EMazeDirection::Up,NodeData.RoomRotation);
		SearchedTags.Add(FName("Wall"));
		UpdateMeshPartVisibilities(SearchedTags, true);
		
		SearchedTags = GetMeshPartNames(EMazeDirection::Up,NodeData.RoomRotation);
		SearchedTags.Add(FName("Door"));
		UpdateMeshPartVisibilities(SearchedTags, false);
	}
	if (NodeData.MazeDirectionMap.Contains(EMazeDirection::Down) && NodeData.MazeDirectionMap[EMazeDirection::Down] != EMazePinType::Hidden)
	{
		TArray<FName> SearchedTags = GetMeshPartNames(EMazeDirection::Down,NodeData.RoomRotation);
		SearchedTags.Add(FName("Door"));
		UpdateMeshPartVisibilities(SearchedTags, true);
		
		SearchedTags = GetMeshPartNames(EMazeDirection::Down,NodeData.RoomRotation);
		SearchedTags.Add(FName("Wall"));
		UpdateMeshPartVisibilities(SearchedTags, false);
		
	}
	else
	{
		TArray<FName> SearchedTags = GetMeshPartNames(EMazeDirection::Down,NodeData.RoomRotation);
		SearchedTags.Add(FName("Wall"));
		UpdateMeshPartVisibilities(SearchedTags, true);
		
		SearchedTags = GetMeshPartNames(EMazeDirection::Down,NodeData.RoomRotation);
		SearchedTags.Add(FName("Door"));
		UpdateMeshPartVisibilities(SearchedTags, false);
	}
	if (NodeData.MazeDirectionMap.Contains(EMazeDirection::Left) && NodeData.MazeDirectionMap[EMazeDirection::Left] != EMazePinType::Hidden)
	{
		TArray<FName> SearchedTags = GetMeshPartNames(EMazeDirection::Left,NodeData.RoomRotation);
		SearchedTags.Add(FName("Door"));
		UpdateMeshPartVisibilities(SearchedTags, true);
		
		SearchedTags = GetMeshPartNames(EMazeDirection::Left,NodeData.RoomRotation);
		SearchedTags.Add(FName("Wall"));
		UpdateMeshPartVisibilities(SearchedTags, false);
	}
	else
	{
		TArray<FName> SearchedTags = GetMeshPartNames(EMazeDirection::Left,NodeData.RoomRotation);
		SearchedTags.Add(FName("Wall"));
		UpdateMeshPartVisibilities(SearchedTags, true);
		
		SearchedTags = GetMeshPartNames(EMazeDirection::Left,NodeData.RoomRotation);
		SearchedTags.Add(FName("Door"));
		UpdateMeshPartVisibilities(SearchedTags, false);
	}
	if (NodeData.MazeDirectionMap.Contains(EMazeDirection::Right) && NodeData.MazeDirectionMap[EMazeDirection::Right] != EMazePinType::Hidden)
	{
		TArray<FName> SearchedTags = GetMeshPartNames(EMazeDirection::Right,NodeData.RoomRotation);
		SearchedTags.Add(FName("Door"));
		UpdateMeshPartVisibilities(SearchedTags, true);
		
		SearchedTags = GetMeshPartNames(EMazeDirection::Right,NodeData.RoomRotation);
		SearchedTags.Add(FName("Wall"));
		UpdateMeshPartVisibilities(SearchedTags, false);
	}
	else
	{
		TArray<FName> SearchedTags = GetMeshPartNames(EMazeDirection::Right,NodeData.RoomRotation);
		SearchedTags.Add(FName("Wall"));
		UpdateMeshPartVisibilities(SearchedTags, true);
		
		SearchedTags = GetMeshPartNames(EMazeDirection::Right,NodeData.RoomRotation);
		SearchedTags.Add(FName("Door"));
		UpdateMeshPartVisibilities(SearchedTags, false);
	}
}

void AMazeTileLevelInstance::GroupActors()
{
	/*
	#if WITH_EDITOR 
	if (LevelStreamingDynamic)
	{
		
		FString Name = GetActorLabel();
		FString CustomPath = FString::Printf(TEXT("Procedural/%s"), *Name);
		this->SetFolderPath(FName(CustomPath));
		if (ULevel* LoadedLevel = LevelStreamingDynamic->GetLoadedLevel())
		{
			for (AActor* Actor : LoadedLevel->Actors)
			{
				Actor->SetFolderPath(FName(CustomPath));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UpdateMeshPartVisibilities: Level not loaded."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UpdateMeshPartVisibilities: LevelStreamingDynamic not loaded."));
	}
	#endif
	*/
}

void AMazeTileLevelInstance::OnEditorLevelLoadedAndShown()
{
	if (LevelStreamingDynamic && LevelStreamingDynamic->IsLevelVisible())
	{
		ULevel* LoadedLevel = LevelStreamingDynamic->GetLoadedLevel();
		UE_LOG(LogTemp, Warning, TEXT("Editor Level Loaded : %s"), *LevelStreamingDynamic->GetWorldAssetPackageName());
		LevelStreamingDynamic->OnLevelShown.RemoveAll(this);
		bLevelLoadedAndShown = true;
		GroupActors();
		ApplyMazeTileData();
	}
	else
	{
		FTimerHandle DummyHandle;
		GetWorld()->GetTimerManager().SetTimer(DummyHandle, this, &ThisClass::OnEditorLevelLoadedAndShown, 1.0f, /*bLooping=*/ false);
	}
}

void AMazeTileLevelInstance::LoadLevelAsync()
{
	bool bSuccess;
	FVector Loc = GetActorLocation();
	FRotator Rot = GetActorRotation();
	UE_LOG(LogTemp, Warning, TEXT("----->: Actor: %s | Location: %s | Rotation: %s"), 
	*GetName(), 
	*Loc.ToString(), 
	*Rot.ToString());
	LevelStreamingDynamic = Cast<ULevelStreamingDynamic>(ULevelStreamingDynamic::LoadLevelInstance(this, LevelName, Loc, Rot, bSuccess, L"", ULevelStreamingDynamic::StaticClass()));
	if (bSuccess)
	{
		LevelStreamingDynamic->OnLevelShown.AddDynamic(this, &AMazeTileLevelInstance::OnLevelLoadedAndShown);
		#if WITH_EDITOR 
			OnEditorLevelLoadedAndShown();
		#endif
	}
}

void AMazeTileLevelInstance::OnLevelLoadedAndShown()
{
	if (LevelStreamingDynamic)
	{
		ULevel* LoadedLevel = LevelStreamingDynamic->GetLoadedLevel();
		UE_LOG(LogTemp, Warning, TEXT("Level Loaded : %s"), *LevelStreamingDynamic->GetWorldAssetPackageName());
		LevelStreamingDynamic->OnLevelShown.RemoveAll(this);
		bLevelLoadedAndShown = true;
		ApplyMazeTileData();
	}
}
