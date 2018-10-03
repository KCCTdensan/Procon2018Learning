#pragma once

#include "Game.hpp"


class battle_field
{
	stage Stage;

public:
	battle_field();
	~battle_field();
	void Battle(int NumTurn);
};