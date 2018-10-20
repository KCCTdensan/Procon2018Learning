#pragma once

#include "General.hpp"
#include "Panel.hpp"
#include "Position.hpp"


class agent
{
	position Position;

public:
	agent();
	~agent();

	void Init(char PositionX, char PositionY);
	void Move(char DeltaX, char DeltaY);
	void Move(intention Intention);
	void Move(action_id ActionID);
	position GetPosition()const;
};