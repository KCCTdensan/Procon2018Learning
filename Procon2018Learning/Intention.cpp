#include "Intention.hpp"


intention::intention()
{
	DeltaX = DeltaY = 0;
}

intention::intention(action_id ID)
{
	switch(ID)
	{
	case ID_TopLeft:
	case ID_Top:
	case ID_TopRight:
		DeltaY = -1;
		break;

	case ID_BottomLeft:
	case ID_Bottom:
	case ID_BottomRight:
		DeltaY = 1;
		break;

	default:
		DeltaY = 0;
		break;
	}
	switch(ID)
	{
	case ID_TopLeft:
	case ID_Left:
	case ID_BottomLeft:
		DeltaX = -1;
		break;

	case ID_TopRight:
	case ID_Right:
	case ID_BottomRight:
		DeltaX = 1;
		break;

	default:
		DeltaX = 0;
		break;
	}
}