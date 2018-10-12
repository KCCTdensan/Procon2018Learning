#pragma once

#include "Panel.hpp"
#include "Position.hpp"


class agent
{
	team_no Team;
	position Position;

public:
	agent();
	~agent();

	void Init(int PositionX, int PositionY, team_no Team);
	void Move(int DeltaX, int DeltaY);
	position GetPosition();
};