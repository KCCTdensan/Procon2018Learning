#pragma once

#include "Game.hpp"


class BattleField
{
	stage Stage;

public:
	BattleField();
	~BattleField();
	void Battle(int NumTurn);
};