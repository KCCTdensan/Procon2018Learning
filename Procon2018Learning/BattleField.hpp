#pragma once

#include "Game.hpp"
#include "Search.hpp"


class battle_field
{
	stage Stage;
	node CurrentNode;

public:
	battle_field();
	~battle_field();
	void Battle(int NumTurn);
};