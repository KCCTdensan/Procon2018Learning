#pragma once

#include "Game.hpp"
#include <vector>


class node
{
	stage Stage;
	int Q;
	int N;
	team_no Team;
	node *Parent;
	std::vector<node*> Child;

public:
	node(stage &Stage);
	void Expansion(team_no Team);
};