#pragma once

constexpr float TILE_SCALE = 400.0f;
constexpr float TILE_EDITOR_SCALE = 100.0f;

UENUM(BlueprintType)
enum class EMazeDirection : uint8
{
	Up,
	Down,
	Left,
	Right,
	None
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