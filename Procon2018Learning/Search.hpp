#pragma once

#include "Game.hpp"
#include <vector>
#include <random>


class node
{
public:
	const static int NumCallPlay = 10000;
	const static int Threshold = 100;

private:
	static std::random_device rand;
	node *Parent;
	unsigned short NumChildren;
	node *Child[Max_ActionID][Max_ActionID];
	stage Stage;
	float Q;
	int N;
	int Record;
	//int NumWin;
	//int NumLose;
	team_no Team;
	action_id IntentionID1;
	action_id IntentionID2;

	int Play();
	int Selection();
	void Expansion();
	int Evaluation();
	int Rollout(stage Stage, int NumTurn);
	float Cost(int Ns);
	bool IsLeafNode();

public:
	node(node *Parent, stage &Stage, team_no Team);
	~node();
	void Search(int NumCallPlay, int(&Result)[Max_ActionID][Max_ActionID]);
};