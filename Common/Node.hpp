#pragma once

#include "Game.hpp"
#include <Windows.h>


const static int NumCallPlay = 100000;
const static int Threshold = 1200;
const static float Cp = 2.0f;

class node
{
protected:
	static CRITICAL_SECTION cs;
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
	static void Init();
	static void Deinit();
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
	friend_node(stage &Stage);
	friend_node(stage &Stage, unsigned char CntTurns);
	~friend_node();
	void Search(int NumCallPlay);
	void Result(int(&Result)[ID_MaxID][ID_MaxID]);
	opponent_node* ChildNode(action_id IntentionID1, action_id IntentionID2);
	opponent_node* ChildNode(action_id(&IntentionIDs)[stage::NumAgents]);
	friend_node* UpdateCurrentNode(action_id(&SelectIntentionIDs)[NumTeams][stage::NumAgents]);
	friend_node* NextNode(action_id(&SelectIntentionIDs)[NumTeams][stage::NumAgents]);
	const stage& GetStage()const;

	void PrintStage()const;
	void PrintChildNodeInfo()const;
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
	opponent_node(friend_node *ParentNode, action_id(&Intentions)[stage::NumAgents], unsigned char CntTurns);
	~opponent_node();
	void Search(int NumCallPlay);
	void Result(int(&Result)[ID_MaxID][ID_MaxID]);
	friend_node* ChildNode(action_id IntentionID1, action_id IntentionID2);
	friend_node* ChildNode(action_id(&IntentionIDs)[stage::NumAgents]);

	void PrintChildNodeInfo()const;
};