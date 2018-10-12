#include "Position.hpp"


position position::operator+=(intention Intention)
{
	if(Intention.Action == IA_RemovePanel)
	{
		return *this;
	}
	x += Intention.DeltaX;
	y += Intention.DeltaY;
	return *this;
}

position position::operator+=(action_id ActionID)
{
	switch(ActionID)
	{
	case Move_TopLeft:
	case Move_Top:
	case Move_TopRight:
		y--;
		break;

	case Move_BottomLeft:
	case Move_Bottom:
	case Move_BottomRight:
		y++;
		break;
	}
	switch(ActionID)
	{
	case Move_TopLeft:
	case Move_Left:
	case Move_BottomLeft:
		x--;
		break;

	case Move_TopRight:
	case Move_Right:
	case Move_BottomRight:
		x++;
		break;
	}
	return *this;
}

position position::operator<<=(intention Intention)
{
	x += Intention.DeltaX;
	y += Intention.DeltaY;
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

position operator<<(position Position, intention Intention)
{
	return position(Position) <<= Intention;
}

bool operator==(position Position1, position Position2)
{
	return Position1.x == Position2.x&&Position1.y == Position2.y;
}

bool operator!=(position Position1, position Position2)
{
	return !(Position1 == Position2);
}