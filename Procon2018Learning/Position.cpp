#include "Position.hpp"


position position::operator+=(intention Intention)
{
	x += Intention.DeltaX;
	y += Intention.DeltaY;
	return *this;
}

position position::operator+=(action_id ActionID)
{
	switch(ActionID)
	{
	case ID_TopLeft:
	case ID_Top:
	case ID_TopRight:
		y--;
		break;

	case ID_BottomLeft:
	case ID_Bottom:
	case ID_BottomRight:
		y++;
		break;
	}
	switch(ActionID)
	{
	case ID_TopLeft:
	case ID_Left:
	case ID_BottomLeft:
		x--;
		break;

	case ID_TopRight:
	case ID_Right:
	case ID_BottomRight:
		x++;
		break;
	}
	return *this;
}


position operator+(position Position, intention Intention)
{
	return position(Position) += Intention;
}

position operator+(position Position, action_id ActionID)
{
	return position(Position) += ActionID;
}

bool operator==(position Position1, position Position2)
{
	return Position1.x == Position2.x&&Position1.y == Position2.y;
}

bool operator!=(position Position1, position Position2)
{
	return !(Position1 == Position2);
}