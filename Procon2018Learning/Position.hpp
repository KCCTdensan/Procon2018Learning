#pragma once

#include "Intention.hpp"


struct position
{
	char x;
	char y;
	position operator+=(intention);
	position operator+=(action_id);
};

position operator+(position, intention);
position operator+(position, action_id);
bool operator==(position, position);
bool operator!=(position, position);