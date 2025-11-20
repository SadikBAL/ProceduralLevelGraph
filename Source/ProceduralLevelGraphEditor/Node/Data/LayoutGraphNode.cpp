#include "LayoutGraphNode.h"

ULayoutGraphNode::ULayoutGraphNode()
{
	CommentColor = FLinearColor::White;
	bCommentBubbleVisible_InDetailsPanel = false;
	NodeWidth = 400;
	NodeHeight = 400;
}
FText ULayoutGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString("Layout Graph Node");
}
void ULayoutGraphNode::AllocateDefaultPins()
{

}