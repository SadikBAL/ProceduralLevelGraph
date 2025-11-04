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
    UPROPERTY()
    TObjectPtr<UEdGraph> EdGraph;

    UFUNCTION()
    void SpawnMazeToEdtior();

    UFUNCTION()
    void SpawnNode(UWorld* World, UMazeNodeBase* MazeNodeBase, EMazeDirection Direction, FVector Location);

    UFUNCTION(CallInEditor, Category = "Procedural Level Graph",DisplayName="Create Maze")
    void CreateMaze();
#endif
    
    UPROPERTY(BlueprintReadOnly, Category = "Procedural Level Graph")
    TArray<TObjectPtr<UMazeNodeBase>> AllNodes;
    
    UPROPERTY(BlueprintReadOnly, Category = "Procedural Level Graph")
    TObjectPtr<UMazeNodeBase> StartNode;

    UPROPERTY()
    TArray<UMazeNodeBase*> Nodes;

    UFUNCTION(BlueprintCallable, Category = "Procedural Level Graph")
    void SpawnMaze(UObject* WorldContextObject);

};