#pragma once

#include "Game.hpp"
#include "Search.hpp"


class battle_field
{
	stage Stage;
	node *CurrentNode;

	void DeepenNode(action_id Action1, action_id Action2);

public:
	battle_field(stage Stg);
	~battle_field();
	void Battle(int NumTurn);
};