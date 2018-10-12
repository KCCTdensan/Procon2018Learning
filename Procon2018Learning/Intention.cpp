#include "Intention.hpp"


intention::intention()
{
	DeltaX = DeltaY = 0;
	Action = IA_MoveAgent;
}

intention::intention(action_id ID)
{
	switch(ID)
	{
	case Move_TopLeft:
	case Remove_TopLeft:
	case Move_Top:
	case Remove_Top:
	case Move_TopRight:
	case Remove_TopRight:
		DeltaY = -1;
		break;

	case Move_BottomLeft:
	case Remove_BottomLeft:
	case Move_Bottom:
	case Remove_Bottom:
	case Move_BottomRight:
	case Remove_BottomRight:
		DeltaY = 1;
		break;

	default:
		DeltaY = 0;
		break;
	}
	switch(ID)
	{
	case Move_TopLeft:
	case Remove_TopLeft:
	case Move_Left:
	case Remove_Left:
	case Move_BottomLeft:
	case Remove_BottomLeft:
		DeltaX = -1;
		break;

	case Move_TopRight:
	case Remove_TopRight:
	case Move_Right:
	case Remove_Right:
	case Move_BottomRight:
	case Remove_BottomRight:
		DeltaX = 1;
		break;

	default:
		DeltaX = 0;
		break;
	}
	switch(ID)
	{
	case Remove_TopLeft:
	case Remove_Top:
	case Remove_TopRight:
	case Remove_Left:
	case Remove_Right:
	case Remove_BottomLeft:
	case Remove_Bottom:
	case Remove_BottomRight:
		Action = IA_RemovePanel;
		break;

	default:
		Action = IA_MoveAgent;
		break;
	}
}