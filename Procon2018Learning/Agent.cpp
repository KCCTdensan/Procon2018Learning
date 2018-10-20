#include "Agent.hpp"


agent::agent()
{

}

agent::~agent()
{

}

void agent::Init(char PositionX, char PositionY)
{
	Position.x = PositionX;
	Position.y = PositionY;
}

void agent::Move(char DeltaX, char DeltaY)
{
	Position.x += DeltaX;
	Position.y += DeltaY;
}

void agent::Move(intention Intention)
{
	Move(Intention.DeltaX, Intention.DeltaY);
}

void agent::Move(action_id ActionID)
{
	Move((intention)ActionID);
}

position agent::GetPosition()const
{
	return Position;
}