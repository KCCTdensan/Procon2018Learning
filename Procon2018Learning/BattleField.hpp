#pragma once

#include "Game.hpp"
#include "TreeSearchAI.hpp"


class battle_field
{
	tree_search_ai AI;

public:
	battle_field(stage &Stage);
	~battle_field();
	void Battle(int NumTurns);
	void Play();
};