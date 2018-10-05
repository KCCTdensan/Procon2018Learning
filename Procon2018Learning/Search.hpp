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
	action_id Intention1;
	action_id Intention2;
	static std::random_device rand;

	bool CanMove(action_id No, position Position);

public:
	node(node *Parent, stage &Stage);
	~node();
	void Search(stage Stage);
	void Selection();
	void Expansion(team_no Team);
	void Evaluation();
	bool Rollout(stage Stage, int NumTurn);
	bool IsLeafNode();
};