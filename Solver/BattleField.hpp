#pragma once

#include "Game.hpp"
#include "Node.hpp"


class battle_field
{
	stage Stages[80];
	friend_node* CurrentNodes[80];
	int intoCard(action_id _IntentionID);
	int intoID(int _Card);

public:
	battle_field();
	~battle_field();
	void Battle(int NumTurn);
};