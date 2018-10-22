#pragma once

#include "Game.hpp"
#include "TreeSearchAI.hpp"


class battle_field
{
	stage *Stages[80];
	friend_node *CurrentNodes[80];

public:
	battle_field(stage &Stage);
	~battle_field();
	void Battle(int NumTurn);
	void Play();
};