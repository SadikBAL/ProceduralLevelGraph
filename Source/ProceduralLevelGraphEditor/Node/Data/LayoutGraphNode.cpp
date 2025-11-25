#include "LayoutGraphNode.h"

#include "ProceduralLevelGraphRuntime/ProceduralLevelGraphTypes.h"

ULayoutGraphNode::ULayoutGraphNode()
{
	CommentColor = FLinearColor::White;
	bCommentBubbleVisible_InDetailsPanel = false;
	NodeWidth = 400;
	NodeHeight = 400;
	bCanRenameNode = true;
	TitleText = FText::FromString("Layout Graph Node");
}

void ULayoutGraphNode::AllocateDefaultPins()
{
}

void ULayoutGraphNode::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(ULayoutGraphNode, BackgroundImage))
	{
		if (BackgroundImage)
		{
			NodeWidth = BackgroundImage->GetSizeX() * TILE_EDITOR_SCALE * 0.1f;
			NodeHeight = BackgroundImage->GetSizeY() * TILE_EDITOR_SCALE * 0.1f;
		}
	}
}

void ULayoutGraphNode::ResizeNode(const FVector2f& NewSize)
{
	if (BackgroundImage)
	{
		NodeWidth = BackgroundImage->GetSizeX() * TILE_EDITOR_SCALE * 0.1f;
		NodeHeight = BackgroundImage->GetSizeY() * TILE_EDITOR_SCALE * 0.1f;
	}
	else
	{
		Super::ResizeNode(NewSize);
	}
}

FText ULayoutGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return TitleText;
}

FText ULayoutGraphNode::GetTooltipText() const
{
	return TitleText;
}
