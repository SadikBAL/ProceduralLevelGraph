#pragma once

#include "ProceduralLevelGraphTypes.generated.h"

class APassagePoint;
constexpr float TILE_SCALE = 100.0f;
constexpr float TILE_EDITOR_SCALE = 20.0f;
constexpr float GRID_SNAP_SCALE = 10.0f;
constexpr float CONNECTION_THRESHOLD = 12.0f;
constexpr float TILE_EDITOR_PIN_SCALE = 16.0f;
constexpr int   MAZE_DIRECTION_MAX = 4;
constexpr int	EMPTY_SIZE = 4;

#if WITH_EDITOR
#define UE_LOG_EDITOR(CategoryName, Verbosity, Format, ...) UE_LOG(CategoryName, Verbosity, Format, ##__VA_ARGS__)
#else
#define UE_LOG_EDITOR(CategoryName, Verbosity, Format, ...)
#endif

UENUM(BlueprintType)
enum class EMazeDirection : uint8
{
	Up = 0,
	Down = 2,
	Left = 3,
	Right = 1,
	None = 4
};
UENUM(BlueprintType)
enum class EMazeOrientation : uint8
{
	Vertical,
	Horizontal,
	None
};
UENUM(BlueprintType)
enum class EMazePinType : uint8
{
	Hidden,
	Closed,
	Opened,
	Minigame,
	Destroyed
};
UENUM(BlueprintType)
enum class EPassageType : uint8
{
	Wall,
	Door,
	Empty
};
USTRUCT(BlueprintType)
struct FPassageActorList
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Passage")
	TArray<AActor*> Actors;
};
UENUM(BlueprintType)
enum class EMazeFloor : uint8
{
	FloorP3 = 0,
	FloorP2 = 1,
	FloorP1 = 2,
	Floor0 = 3,
	Floor1 = 4,
	Floor2 = 5,
	Floor3 = 6,
	Error
};
UENUM(BlueprintType)
enum class ERouteType : uint8
{
	RouteA,
	RouteB,
	RouteC,
	RouteD
};


USTRUCT(BlueprintType)
struct FDoorData
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere, Category = "Door Data")
	EMazeDirection DoorLocation = EMazeDirection::Down;
	UPROPERTY(EditAnywhere, Category = "Door Data")
	EMazePinType DoorStatus = EMazePinType::Closed;
	UPROPERTY(VisibleAnywhere, Category = "Door Data")
	FVector2D DoorOffset = FVector2D::ZeroVector;
	UPROPERTY(VisibleAnywhere, Category = "Door Data")
	EMazeFloor DoorFloor = EMazeFloor::Floor0;
	UPROPERTY(VisibleAnywhere, Category = "Door Data")
	EMazeOrientation DoorDirection = EMazeOrientation::Horizontal;
	UPROPERTY(VisibleAnywhere, Category = "Door Data")
	TObjectPtr<class UMazeNodeBase> LinkedNode;
	UPROPERTY(VisibleAnywhere, Category = "Door Data")
	int Offset = 0;;
	//UPROPERTY(VisibleAnywhere, Category = "Door Data")
	//TObjectPtr<const APassagePoint> PassagePoint;
	
};

USTRUCT(BlueprintType)
struct FMazeTileData
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Maze Nodes")
	TArray<FDoorData> DoorData;
	//UPROPERTY(VisibleAnywhere, Category = "Maze Node")
	//TMap<EMazeDirection, EMazePinType> MazeDirectionMap;

	//UPROPERTY(VisibleAnywhere, Category = "Maze Node")
	//int32 RoomRotation = 0;
	
	//UPROPERTY(VisibleAnywhere, Category = "Maze Node")
	//FBox RoomBounds = FBox();
};
constexpr int	PROCEDURAL_HEIGHT_MIN = -18000;
constexpr int	PROCEDURAL_HEIGHT_MAX =  18000;

static int32 GetFloorHeight(EMazeFloor FloorType)
{
	switch (FloorType)
	{
		case EMazeFloor::Floor0:	return 0;
		case EMazeFloor::Floor1:    return 600;
		case EMazeFloor::Floor2:	return 1200;
		case EMazeFloor::Floor3:    return 1800;
		case EMazeFloor::FloorP1:	return -600;
		case EMazeFloor::FloorP2:	return -1200;
		case EMazeFloor::FloorP3:	return -1800;
		default: return 9999;
	}
}
static EMazeFloor AddFloor(EMazeFloor RoomFloor, EMazeFloor DoorFloor)
{
	int32 FloorA = static_cast<int32>(RoomFloor);
	int32 FloorB = static_cast<int32>(DoorFloor);
	int32 FloorCenter = static_cast<int32>(EMazeFloor::Floor0);
	int32 FloorResult = FloorA + (FloorB - FloorCenter);
	return static_cast<EMazeFloor>(FloorResult);
}
static FSlateColor GetPinColorWithHeight(EMazeFloor FloorType)
{
	float Height = GetFloorHeight(FloorType);
	if (Height >= 9999)
		return FSlateColor(FLinearColor::Black);
	float Alpha = (Height + PROCEDURAL_HEIGHT_MAX) / PROCEDURAL_HEIGHT_MAX * 2;
	FLinearColor FinalColor;

	if (Alpha < 0.5f)
	{
		float SubAlpha = Alpha * 2.0f;
		FinalColor = FLinearColor::LerpUsingHSV(FLinearColor::Blue, FLinearColor::Green, SubAlpha);
	}
	else if (Alpha > 0.5f)
	{
		float SubAlpha = (Alpha - 0.5f) * 2.0f;
		FinalColor = FLinearColor::LerpUsingHSV(FLinearColor::Green, FLinearColor::Red, SubAlpha);
	}
	else
	{
		FinalColor = FLinearColor::Green;
	}
	return FSlateColor(FinalColor);
}