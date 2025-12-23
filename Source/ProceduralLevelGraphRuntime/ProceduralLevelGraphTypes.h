#pragma once

#include "ProceduralLevelGraphTypes.generated.h"

constexpr float TILE_SCALE = 100.0f;
constexpr float TILE_EDITOR_SCALE = 25.0f;
constexpr float GRID_SNAP_SCALE = 25.0f;
constexpr float TILE_EDITOR_PIN_SCALE = 24.0f;
constexpr int   MAZE_DIRECTION_MAX = 4;

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
	Tier1
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
	
	UPROPERTY(EditAnywhere, Category = "Door Data")
	EMazeDirection DoorType;
	UPROPERTY(EditAnywhere, Category = "Door Data")
	EMazePinType DoorVisibility;
	UPROPERTY(EditAnywhere, Category = "Door Data")
	FVector2D DoorOffset;
};