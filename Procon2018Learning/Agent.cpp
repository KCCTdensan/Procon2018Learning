#include<iostream>
#include "Agent.hpp"


agent::agent()
{

}

agent::~agent()
{

}

void agent::Init(char PositionX, char PositionY, team_no Team)
{
	Position.x = PositionX;
	Position.y = PositionY;
	std::cout << (int)PositionX << "," << (int)PositionY << std::endl;
	this->Team = Team;
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

position agent::GetPosition()
{
	return Position;
}