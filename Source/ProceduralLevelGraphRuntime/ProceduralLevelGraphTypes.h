#pragma once


#include "ProceduralLevelGraphTypes.generated.h"

constexpr float TILE_SCALE = 100.0f;
constexpr float TILE_EDITOR_SCALE = 25.0f;
constexpr float GRID_SNAP_SCALE = 5.0f;
constexpr float TILE_EDITOR_PIN_SCALE = 24.0f;
constexpr int   MAZE_DIRECTION_MAX = 4;
constexpr int	EMPTY_SIZE = 5;

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
enum class EMazeFloor : uint8
{
	FloorP3,
	FloorP2,
	FloorP1,
	Floor0,
	Floor1,
	Floor2,
	Floor3
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
struct FMazeTileData
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, Category = "Maze Node")
	TMap<EMazeDirection, EMazePinType> MazeDirectionMap;

	UPROPERTY(VisibleAnywhere, Category = "Maze Node")
	int32 RoomRotation = 0;
	
	UPROPERTY(VisibleAnywhere, Category = "Maze Node")
	FBox RoomBounds = FBox();
};

USTRUCT(BlueprintType)
struct FDoorData
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, Category = "Door Data")
	EMazeDirection DoorLocation;
	UPROPERTY(EditAnywhere, Category = "Door Data")
	EMazePinType DoorStatus;
	UPROPERTY(VisibleAnywhere, Category = "Door Data")
	FVector2D DoorOffset;
	UPROPERTY(EditAnywhere, Category = "Door Data")
	EMazeFloor DoorFloor;
	UPROPERTY(VisibleAnywhere, Category = "Door Data")
	EMazeOrientation DoorDirection;
	UPROPERTY()
	TObjectPtr<class UMazeNodeBase> LinkedNode;

};

constexpr int	PROCEDURAL_HEIGHT_MIN = -18000;
constexpr int	PROCEDURAL_HEIGHT_MAX =  18000;

static int32 GetFloorHeight(EMazeFloor FloorType)
{
	switch (FloorType)
	{
		case EMazeFloor::Floor0:		return 0;
		case EMazeFloor::Floor1:     return 6000;
		case EMazeFloor::Floor2:		return 12000;
		case EMazeFloor::Floor3:     return 18000;
		case EMazeFloor::FloorP1:	return -6000;
		case EMazeFloor::FloorP2:	return -12000;
		case EMazeFloor::FloorP3:	return -18000;
		default: return 0;
	}
}
static FSlateColor GetPinColorWithHeight(EMazeFloor FloorType)
{
	float Height = GetFloorHeight(FloorType);
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