#pragma once

#include "Game.hpp"


class node
{
public:
	const static int NumCallPlay = 30000;
	const static int Threshold = 100;

protected:
	const unsigned char NumTurns;
	unsigned char NumChildren;
	float Q;
	int N;
	int Record;

	float UCB1(float Q, int NChild);
	bool IsLeafNode();

public:
	node(unsigned char NumTurns);
	~node();
};

class opponent_node;

class friend_node :public node
{
	friend opponent_node;

	opponent_node *ParentNode;
	opponent_node *Children[ID_MaxID][ID_MaxID];
	stage Stage;

	int Play();
	int Selection();
	void Expansion();
	int Evaluation();
	int Rollout(int NumTurn);
	void ClearChildNode();

public:
	friend_node(opponent_node *ParentNode, stage Stage, unsigned char NumTurns);
	~friend_node();
	void Search(int NumCallPlay, int(&Result)[ID_MaxID][ID_MaxID]);
	friend_node* UpdateCurrentNode(action_id(&SelectIntentionIDs)[NumTeams][stage::NumAgents]);
	void PrintStage();
};

class opponent_node :public node
{
	friend friend_node;

	friend_node *ParentNode;
	friend_node *Children[ID_MaxID][ID_MaxID];
	action_id FriendIntentionIDs[stage::NumAgents];

	int Play();
	int Selection();
	void Expansion();
	void ClearChildNode();

public:
	opponent_node(friend_node *ParentNode, action_id(&Intentions)[stage::NumAgents], unsigned char NumTurns);
	~opponent_node();
};