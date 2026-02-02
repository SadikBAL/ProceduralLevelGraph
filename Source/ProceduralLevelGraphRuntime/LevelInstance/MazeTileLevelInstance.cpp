#include "MazeTileLevelInstance.h"

// #include "FileHelpers.h"
#include "LevelBound.h"
#include "LevelInstanceManagerComponent.h"
#include "PassagePoint.h"
#include "Components/PointLightComponent.h"
#include "ProceduralLevelGraphRuntime/ProceduralLevelGraphTypes.h"
#include "ProceduralLevelGraphRuntime/Node/HallNode.h"
#include "ProceduralLevelGraphRuntime/Node/MazeNodeBase.h"

#if WITH_EDITOR
#include "UObject/ObjectSaveContext.h"
#endif

class UHallNode;

AMazeTileLevelInstance::AMazeTileLevelInstance()
{
	LevelInstanceManager = CreateDefaultSubobject<
		ULevelInstanceManagerComponent>(TEXT("LevelInstanceManagerComponent"));
	LevelInstanceManager->LevelInstance = this;
	LevelInstanceManager->SetAutoActivate(true);
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
	LevelName = WorldAsset.ToSoftObjectPath().GetLongPackageName();
	TArray<AActor*> IgnoreList;
	LoadMapData(IgnoreList);
}

void AMazeTileLevelInstance::LoadMapData(TArray<AActor*>& IgnoreList)
{
	UE_LOG(LogTemp, Log, TEXT("AMazeTileLevelInstance::LoadMapData()"));
	DoorData.Empty();
	if (UPackage* Package = LoadPackage(nullptr, *LevelName, LOAD_None))
	{
		if (const UWorld* World = UWorld::FindWorldInPackage(Package))
		{
			for (const AActor* Actor : World->PersistentLevel->Actors)
			{
				if (IgnoreList.Contains(Actor))
				{
					continue;
				}
				//Update LevelBound Data.
				if (const ALevelBound* LevelBounds = Cast<ALevelBound>(Actor))
				{
					Width = LevelBounds->Width;
					Height = LevelBounds->Height;
				}
				else if (const APassagePoint* PassagePoint = Cast<APassagePoint>(Actor))
				{
					FDoorData TempData;
					TempData.PassageSize = PassagePoint->PassageSize;
					TempData.DoorFloor = PassagePoint->DoorFloor;
					TempData.Offset = PassagePoint->Offset;
					//TempData.PassagePoint = PassagePoint;
					if (PassagePoint->GetActorRotation().Yaw == 0)
					{
						TempData.DoorDirection = EMazeOrientation::Vertical;
						//To-Do DoorStatus must be remembered.
						TempData.DoorStatus = EMazePinType::Closed;
						TempData.DoorOffset.X = PassagePoint->GetActorLocation().X / 100.0f;;
						TempData.DoorOffset.Y = 0;
						if (PassagePoint->GetActorLocation().Y > 0)
						{
							TempData.DoorLocation = EMazeDirection::Down;
						}
						else
						{
							TempData.DoorLocation = EMazeDirection::Up;
						}
					}
					else
					{
						TempData.DoorDirection = EMazeOrientation::Horizontal;
						TempData.DoorStatus = EMazePinType::Closed;
						TempData.DoorOffset.Y = PassagePoint->GetActorLocation().Y / 100.0f;;
						TempData.DoorOffset.X = 0;
						if (PassagePoint->GetActorLocation().X > 0)
						{
							TempData.DoorLocation = EMazeDirection::Right;
						}
						else
						{
							TempData.DoorLocation = EMazeDirection::Left;
						}
					}
					DoorData.Add(TempData);
				}
			}
		}
	}
	MarkPackageDirty();
	OnMazeTileLevelInstanceUpdated.Broadcast();
}

void AMazeTileLevelInstance::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

#endif

void AMazeTileLevelInstance::SetNodeData(UMazeNodeBase* BaseNode)
{
	TileData.DoorData = BaseNode->DoorData;
	RoomTags = BaseNode->RoomTags;
	LoadLevelAsync();
}

void AMazeTileLevelInstance::SetNodeDataFromHall(UMazeNodeBase* BaseNode, EHallPartType HallPartType)
{
	TileData.bHallTile = true;
	TileData.HallPartType = HallPartType;
	SetNodeData(BaseNode);
}

void AMazeTileLevelInstance::ApplyMazeTileData()
{
	if (ULevel* LoadedLevel = LevelStreamingDynamic->GetLoadedLevel())
	{
		for (AActor* Actor : LoadedLevel->Actors)
		{
			if (APassagePoint* PassagePoint = Cast<APassagePoint>(Actor))
			{
				for (FDoorData Door : TileData.DoorData)
				{
					if (TileData.bHallTile)
					{
						if (Door.LinkedNode)
						{
							EPassageSize PassageSize = Door.LinkedNode->GetConnectedPassageSize(Door.OwnerNode);
							if (PassageSize == EPassageSize::Double)
							{
								PassagePoint->UpdatePassageStatus(EPassageType::Door_Double);
							}
							else if (PassageSize == EPassageSize::Single)
							{
								PassagePoint->UpdatePassageStatus(EPassageType::Door);
							}
							else if (PassageSize == EPassageSize::Vertical)
							{
								PassagePoint->UpdatePassageStatus(EPassageType::Door_Vertical);
							}
						}
						else
						{
							PassagePoint->UpdatePassageStatus(EPassageType::Wall);
						}
					}
					else if (PassagePoint->IsPassageDataMatchDoorData(Door))
					{
						if (Door.LinkedNode)
						{
							switch (Door.PassageSize)
							{
								case EPassageSize::Double:
									PassagePoint->UpdatePassageStatus(EPassageType::Door_Double);
									break;
								case EPassageSize::Single:
									PassagePoint->UpdatePassageStatus(EPassageType::Door);
									break;
								case EPassageSize::Vertical:
									PassagePoint->UpdatePassageStatus(EPassageType::Door_Vertical);
									break;
							}
						}
						else
						{
							PassagePoint->UpdatePassageStatus(EPassageType::Wall);
						}
					}
				}
			}
		}
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
		GetWorld()->GetTimerManager().SetTimer(DummyHandle, this, &ThisClass::OnEditorLevelLoadedAndShown, 1.0f,
		                                       /*bLooping=*/ false);
	}
}

void AMazeTileLevelInstance::LoadLevelAsync()
{
	bool bSuccess;
	if (LevelName.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("LoadLevelAsync LevelName could not be empty."));
		return;
	}
	LevelStreamingDynamic = Cast<ULevelStreamingDynamic>(
		ULevelStreamingDynamic::LoadLevelInstance(
			this, LevelName, GetActorLocation(), GetActorRotation(), bSuccess, L"",
			ULevelStreamingDynamic::StaticClass()));
	if (bSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Level Loading : %s"), *LevelStreamingDynamic->GetWorldAssetPackageName());
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
