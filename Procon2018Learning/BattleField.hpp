#pragma once

#include "Game.hpp"
#include "Node.hpp"


class battle_field
{
	struct log
	{
		panels Panels[MaxY][MaxX];
		agent Agents[NumTeams][stage::NumAgents];
		int N[9][9];//nullptr‚Í0
	};
public:
	battle_field();
	~battle_field();
	void Battle(int NumTurn);
};