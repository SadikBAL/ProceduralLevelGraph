#pragma once

#include "CoreMinimal.h"
#include "Node/MazeNodeBase.h"
#include "UObject/Object.h"
#include "ProceduralLevelGraphRuntime.generated.h"

// Forward declaration
class UEdGraph;

UCLASS(Blueprintable)
class PROCEDURALLEVELGRAPHRUNTIME_API UProceduralLevelGraphRuntime : public UObject
{
    GENERATED_BODY()

public:
    UProceduralLevelGraphRuntime();

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug Settings")
    bool bShowRoutes = false;
    
    UPROPERTY()
    TObjectPtr<UEdGraph> EdGraph;
#endif
#if WITH_EDITOR
    UFUNCTION()
    void SpawnMazeToEditor();
    
    UFUNCTION(CallInEditor, Category = "Procedural Level Graph",DisplayName="Create Maze")
    void CreateMaze();

    UFUNCTION(CallInEditor, Category = "Procedural Level Graph",DisplayName="Delete Maze")
    void DeleteMaze();

    UFUNCTION(CallInEditor, Category = "Procedural Level Graph",DisplayName="Recreate Maze")
    void RecreateMaze();
#endif
    
    UFUNCTION()
    void SpawnNode(UWorld* World, UMazeNodeBase* MazeNodeBase, EMazeDirection Direction, FVector Location);
    
    UPROPERTY(BlueprintReadOnly, Category = "Procedural Level Graph")
    TArray<TObjectPtr<UMazeNodeBase>> AllNodes;
    
    UPROPERTY(BlueprintReadOnly, Category = "Procedural Level Graph")
    TObjectPtr<UMazeNodeBase> StartNode;

    UPROPERTY()
    TArray<UMazeNodeBase*> Nodes;

    UFUNCTION(BlueprintCallable, Category = "Procedural Level Graph")
    void SpawnMaze(UObject* WorldContextObject);

};