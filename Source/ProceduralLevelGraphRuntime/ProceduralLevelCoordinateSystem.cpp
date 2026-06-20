// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralLevelCoordinateSystem.h"
#include "Node/MazeNodeBase.h"

UProceduralLevelCoordinateSystem::UProceduralLevelCoordinateSystem()
{
}

void UProceduralLevelCoordinateSystem::SaveBounds(UMazeNodeBase* Node)
{
	float NodeMinX = -Node->GetEdgePosition(EMazeDirection::Down).Y;
	if (NodeMinX < MinX) MinX = NodeMinX;
	
	float NodeMaxX = -Node->GetEdgePosition(EMazeDirection::Up).Y;
	if (NodeMaxX > MaxX) MaxX = NodeMaxX;
	
	float NodeMinY = -Node->GetEdgePosition(EMazeDirection::Right).X;
	if (NodeMinY < MinY) MinY = NodeMinY;
	
	float NodeMaxY = -Node->GetEdgePosition(EMazeDirection::Left).X;
	if (NodeMaxY > MaxY) MaxY = NodeMaxY;
}

// Bottom left corner is 0,0 and top right is 1,1
FVector2D UProceduralLevelCoordinateSystem::GetNormalizedCoordinate(FVector WorldLocation)
{
	if (Width == 0 || Height == 0)
	{
		Width = MaxX - MinX;
		Height = MaxY - MinY;
	}
	
	float NormalizedX = -(WorldLocation.Y + MinX) / Width;
	float NormalizedY = -(WorldLocation.X + MinY) / Height;
	
	return FVector2D(NormalizedX, NormalizedY);
}
