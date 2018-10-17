#pragma once

#include "General.hpp"
#include "Panel.hpp"
#include "Position.hpp"


class agent
{
	team_no Team;
	position Position;

public:
	agent();
	~agent();

	void Init(char PositionX, char PositionY, team_no Team);
	void Move(char DeltaX, char DeltaY);
	void Move(intention Intention);
	void Move(action_id ActionID);
	position GetPosition();
};