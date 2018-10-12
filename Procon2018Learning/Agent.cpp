#include "Agent.hpp"


agent::agent()
{

}

agent::~agent()
{

}

void agent::Init(int PositionX, int PositionY, team_no Team)
{
	Position.x = PositionX;
	Position.y = PositionY;
	this->Team = Team;
}

void agent::Move(int DeltaX, int DeltaY)
{
	Position.x += DeltaX;
	Position.y += DeltaY;
}

position agent::GetPosition()
{
	return Position;
}