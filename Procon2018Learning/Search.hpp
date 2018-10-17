#pragma once

#include "Game.hpp"


class node
{
public:
	const static int NumCallPlay = 10000;
	const static int Threshold = 100;

private:
	node *Parent;
	unsigned short NumChildren;
	node *Child[ID_MaxID][ID_MaxID];
	stage Stage;
	float Q;
	int N;
	int Record;
	team_no Team;

	int Play();
	int Selection();
	void Expansion();
	int Evaluation();
	int Rollout(stage Stage, int NumTurn);
	float UCB1(float Q, int NChild);
	bool IsLeafNode();
	void ClearChildNode();

public:
	node(node *Parent, stage &Stage, team_no Team);
	~node();
	void Search(int NumCallPlay, int(&Result)[ID_MaxID][ID_MaxID]);
	node* Deepen(action_id Action1, action_id Action2);
};