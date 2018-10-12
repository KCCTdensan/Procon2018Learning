#pragma once

#include "Game.hpp"
#include <vector>
#include <random>


class node
{
	stage Stage;
	int Q;
	int N;
	team_no Team;
	node *Parent;
	std::vector<node*> Child;
	action_id IntentionID1;
	action_id IntentionID2;
	std::random_device rand;

	

public:
	node(node *Parent, stage &Stage);
	~node();
	void play();
	void Selection();
	int cost(int Ns);
	void Expansion(team_no Team);
	void Evaluation();
	int Rollout(stage Stage, int NumTurn);
	bool IsLeafNode();
};