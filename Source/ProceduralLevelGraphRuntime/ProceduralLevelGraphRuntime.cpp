#include "ProceduralLevelGraphRuntime.h"
#include "HallNode.h"
#include "RoomNode.h"
#include "ProceduralLevelGraphEditor/Room/HallGraphNode.h"
#include "ProceduralLevelGraphEditor/Room/MazeGraphNodeBase.h"
#include "ProceduralLevelGraphEditor/Room/RoomGraphNode.h"

UProceduralLevelGraphRuntime::UProceduralLevelGraphRuntime()
{
}

#if WITH_EDITOR
void UProceduralLevelGraphRuntime::SaveGraphToRuntimeData()
{
    
    if (EdGraph == nullptr) return;
    
    AllNodes.Empty();
    StartNode = nullptr;

    TMap<UEdGraphNode*, UMazeNodeBase*> NodeMap;
    for (UEdGraphNode* EdNode : EdGraph->Nodes)
    {
        if (UMazeGraphNodeBase* MazeEdNode = Cast<UMazeGraphNodeBase>(EdNode))
        {
            UMazeNodeBase* NewRuntimeNode = nullptr;
            
            if (URoomGraphNode* RoomEdNode = Cast<URoomGraphNode>(EdNode))
            {
                URoomNode* RuntimeRoom = NewObject<URoomNode>(this);
                NewRuntimeNode = RuntimeRoom;
            }
            else if (UHallGraphNode* HallEdNode = Cast<UHallGraphNode>(EdNode))
            {
                UHallNode* RuntimeHall = NewObject<UHallNode>(this);
                RuntimeHall->HallLength = HallEdNode->HallLength;
                RuntimeHall->bIsHorizontal = HallEdNode->bHorizentalMode;
                NewRuntimeNode = RuntimeHall;
            }
            
            if (NewRuntimeNode)
            {
                NewRuntimeNode->GameplayTags = MazeEdNode->GameplayTags;
                NewRuntimeNode->NodeGuid = MazeEdNode->NodeGuid;
                
                AllNodes.Add(NewRuntimeNode);
                NodeMap.Add(EdNode, NewRuntimeNode);
                if (StartNode == nullptr)
                {
                    StartNode = NewRuntimeNode;
                }
            }
        }
    }
    
    for (UEdGraphNode* EdNode : EdGraph->Nodes)
    {
        if (!NodeMap.Contains(EdNode)) continue;

        UMazeNodeBase* FromRuntimeNode = NodeMap[EdNode];

        for (UEdGraphPin* Pin : EdNode->Pins)
        {
            if (Pin->Direction == EGPD_Output && Pin->LinkedTo.Num() > 0)
            {
                for (UEdGraphPin* LinkedPin : Pin->LinkedTo)
                {
                    UEdGraphNode* LinkedEdNode = LinkedPin->GetOwningNode();
                    if (NodeMap.Contains(LinkedEdNode))
                    {
                        UMazeNodeBase* ToRuntimeNode = NodeMap[LinkedEdNode];
                        FromRuntimeNode->ConnectedNodes.Add(ToRuntimeNode);
                    }
                }
            }
        }
    }
    this->MarkPackageDirty();
}
#endif