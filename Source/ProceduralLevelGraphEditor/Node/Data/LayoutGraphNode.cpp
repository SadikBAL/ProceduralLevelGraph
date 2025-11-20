#include "LayoutGraphNode.h"

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

FText ULayoutGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return TitleText;
}

void ULayoutGraphNode::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(ULayoutGraphNode, BackgroundImage))
	{
		if (BackgroundImage)
		{
			NodeWidth = BackgroundImage->GetSizeX();
			NodeHeight = BackgroundImage->GetSizeY();
		}
	}
}

void ULayoutGraphNode::ResizeNode(const FVector2f& NewSize)
{
	if (BackgroundImage)
	{
		NodeWidth = BackgroundImage->GetSizeX();
		NodeHeight = BackgroundImage->GetSizeY();
	}
	else
	{
		Super::ResizeNode(NewSize);
	}
}

void ULayoutGraphNode::OnRenameNode(const FString& NewName)
{
	TitleText = FText::FromString(NewName);
	Super::OnRenameNode(NewName);
}