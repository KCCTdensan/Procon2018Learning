#pragma once

#include "Game.hpp"
#include <vector>
#include <random>


class node
{
public:
	const static int NumCallPlay = 10000;

private:
	stage Stage;
	float Q;
	int NumWin;
	int NumLose;
	int N;
	int Threshold;
	team_no Team;
	node *Parent;
	std::vector<node*> Child;
	action_id IntentionID1;
	action_id IntentionID2;
	std::random_device rand;

public:
	node(node *Parent, stage &Stage);
	~node();
	void Search(int NumCallPlay, float(&Result)[Max_ActionID * Max_ActionID]);
	void Play();
	void Selection();
	float Cost(int Ns);
	void Expansion();
	int Rollout(stage Stage, int NumTurn);
	bool IsLeafNode();
};