#pragma once

#include "Game.hpp"
#include "Node.hpp"


class battle_field
{
	//stage Stage;
	stage Stages[80];
	friend_node* CurrentNodes[80];

public:
	battle_field();
	~battle_field();
	void Battle(int NumTurn);
};
