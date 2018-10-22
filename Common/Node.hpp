﻿#pragma once

#include "Game.hpp"


class node
{
public:
	const static int NumCallPlay = 100000;
	const static int Threshold = 100;

protected:
	static unsigned char NumTurns;
	unsigned char CntTurns;
	unsigned char NumChildren;
	float Q;
	int N;
	int Record;

	bool IsLeafNode();

public:
	node(unsigned char CntTurns);
	~node();
	static void ChangeNumTurns(unsigned char NumTurns);
	void ChangeCntTurns(unsigned char CntTurns);
};

class opponent_node;

//==============================================================
//味方が手を選ぶためのノード
//それぞれの子ノード(敵ノード)に味方の選んだ手が入る
//==============================================================
class friend_node :public node
{
	friend opponent_node;

	opponent_node *Children[ID_MaxID][ID_MaxID];
	stage Stage;

	int Play();
	int Selection();
	void Expansion();
	int Evaluation();
	int Rollout(int NumTurn);
	void ClearChildNode();
	float UCB1(float Q, int NChild);

public:
	friend_node(opponent_node *ParentNode, stage &Stage, unsigned char NumTurns);
	~friend_node();
	void Search(int NumCallPlay);
	void Result(int(&Result)[ID_MaxID][ID_MaxID]);
	opponent_node* ChildNode(action_id IntentionID1, action_id IntentionID2);
	opponent_node* ChildNode(action_id(&IntentionIDs)[stage::NumAgents]);
	friend_node* UpdateCurrentNode(action_id(&SelectIntentionIDs)[NumTeams][stage::NumAgents]);

	void PrintStage();
	void PrintChildNodeInfo();
};

//==============================================================
//敵が手を選ぶためのノード
//それぞれの子ノード(味方ノード)に敵の選んだ手が入る
//==============================================================
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
	float UCB1(float Q, int NChild);

public:
	opponent_node(friend_node *ParentNode, action_id(&Intentions)[stage::NumAgents], unsigned char NumTurns);
	~opponent_node();
	void Search(int NumCallPlay);
	void Result(int(&Result)[ID_MaxID][ID_MaxID]);
	friend_node* ChildNode(action_id IntentionID1, action_id IntentionID2);
	friend_node* ChildNode(action_id(&IntentionIDs)[stage::NumAgents]);

	void PrintChildNodeInfo();
};