﻿#include "Node.hpp"
#include "Random.hpp"
#include <iostream>


float node::UCB1(float Q, int NChild)
{
	const static float Cp = 1.0f;
	if(NChild == 0)
	{
		return INFINITY;
	}
	return Q + Cp * std::sqrtf(2.0f * std::logf((float)N) / (float)NChild);
}

bool node::IsLeafNode()
{
	return NumChildren == 0;
}

node::node(unsigned char NumTurns)
	:NumTurns(NumTurns)
{
	NumChildren = 0;
	Q = 0.0;
	N = 0;
	Record = 0;
}

node::~node()
{

}

int friend_node::Play()
{
	int Ret;
	if (N >= Threshold)
	{
		if (IsLeafNode())
		{
			Expansion();
		}
		Ret = Selection();
	}
	else
	{
		Ret = Evaluation();
	}
	N++;
	Record += Ret;
	Q = (float)Record / N;
	return Ret;
}

int friend_node::Selection()
{
	float Q_CMax = -10.0f;
	action_id Selected_i = ID_None, Selected_j = ID_None;
	for (action_id i = 0; i < ID_MaxID; ++i)
	{
		for (action_id j = 0; j < ID_MaxID; ++j)
		{
			if (Children[i][j] == nullptr)
			{
				continue;
			}
			float Q_C = UCB1(Children[i][j]->Q, Children[i][j]->N);
			if (Q_C == INFINITY)
			{
				return Children[i][j]->Play();
			}
			if (Q_CMax < Q_C)
			{
				Q_CMax = Q_C;
				Selected_i = i;
				Selected_j = j;
			}

		}
	}
	if (Selected_i == ID_None)
	{
		Q = -1.0f;
		return -1;
	}
	return Children[Selected_i][Selected_j]->Play();
}

void friend_node::Expansion()
{
	for (action_id i = 0; i < ID_MaxID; ++i)
	{
		if (!Stage.CanAction(i, Team_1P, 0))
		{
			continue;
		}
		for (action_id j = 0; j < ID_MaxID; ++j)
		{
			action_id ID[] = { i,j };
			if (!Stage.CanAction(ID, Team_1P))
			{
				continue;
			}

			Children[i][j] = new opponent_node(this, ID, NumTurns);
			NumChildren++;
		}
	}
}

int friend_node::Evaluation()
{
	return Rollout(stage::MaxTurn - Stage.GetCntTurn());
}

int friend_node::Rollout(int NumTurn)
{
	stage RolloutStage(Stage);
	for (int i = 0; i < NumTurn; ++i)
	{
		action_id IntentionIDs[NumTeams][stage::NumAgents];
		do
		{
			for (team_no t = 0; t < NumTeams; ++t)
			{
				for (char a = 0; a < stage::NumAgents; ++a)
				{
					IntentionIDs[t][a] = (action_id)random::Mod(ID_MaxID - 1) + 1;
				}
			}
		} while (!RolloutStage.CanAction(IntentionIDs));
		RolloutStage.Action(IntentionIDs);
	}
	int Score1P = RolloutStage.GetScore1P();
	int Score2P = RolloutStage.GetScore2P();
	if (Score1P > Score2P)
	{
		return 1;
	}
	if (Score1P < Score2P)
	{
		return -1;
	}
	return 0;
}

void friend_node::ClearChildNode()
{
	for (action_id i = 0; i < ID_MaxID; ++i)
	{
		for (action_id j = 0; j < ID_MaxID; ++j)
		{
			if (Children[i][j] == nullptr)
			{
				continue;
			}
			delete Children[i][j];
			Children[i][j] = nullptr;
		}
	}
}

friend_node::friend_node(opponent_node *ParentNode, stage Stage, unsigned char NumTurns)
	:node(NumTurns), Stage(Stage)
{
	this->ParentNode = ParentNode;
	for (action_id i = 0; i < ID_MaxID; ++i)
	{
		for (action_id j = 0; j < ID_MaxID; ++j)
		{
			Children[i][j] = nullptr;
		}
	}
}

friend_node::~friend_node()
{
	ClearChildNode();
}

void friend_node::Search(int NumCallPlay, int(&Result)[ID_MaxID][ID_MaxID])
{
	if (IsLeafNode())
	{
		Expansion();
	}
	for (int i = 0; i < NumCallPlay; ++i)
	{
		N++;
		Record += Selection();
		Q = (float)Record / N;
	}
	
	for (action_id i = 0; i < ID_MaxID; ++i)
	{
		for (action_id j = 0; j < ID_MaxID; ++j)
		{
			if (Children[i][j] == nullptr)
			{
				Result[i][j] = 0;
				continue;
			}
			Result[i][j] = Children[i][j]->N;
		}
	}
	using namespace std;
	cout << endl;
	cout << "Ns : " << N << endl;
	for (action_id i = 0; i < ID_MaxID; ++i)
	{
		for (action_id j = 0; j < ID_MaxID; ++j)
		{
			if (Children[i][j] == nullptr)
			{
				cout << " ********  ";
				continue;
			}
			printf("[N:%6d] ", Children[i][j]->N);
		}
		cout << endl;
		for (action_id j = 0; j < ID_MaxID; ++j)
		{
			if (Children[i][j] == nullptr)
			{
				cout << " ********  ";
				continue;
			}
			printf("[R:%6d] ", Children[i][j]->Record);
		}
		cout << endl << endl;
	}
}

friend_node* friend_node::UpdateCurrentNode(action_id(&SelectIntentionIDs)[NumTeams][stage::NumAgents])
{
	opponent_node *SelectedOpponentNode = Children[SelectIntentionIDs[Team_1P][0]][SelectIntentionIDs[Team_1P][1]];
	friend_node *SelectedFriendNode = SelectedOpponentNode->Children[SelectIntentionIDs[Team_2P][0]][SelectIntentionIDs[Team_2P][1]];
	SelectedOpponentNode->Children[SelectIntentionIDs[Team_2P][0]][SelectIntentionIDs[Team_2P][1]] = nullptr;
	delete this;
	return SelectedFriendNode;
}

void friend_node::PrintStage()
{
	Stage.PrintStage();
}

int opponent_node::Play()
{
	if (IsLeafNode())
	{
		Expansion();
	}
	int Ret = Selection();
	N++;
	Record -= Ret;
	Q = (float)Record / N;
	return Ret;
}

int opponent_node::Selection()
{
	float Q_CMax = -10.0f;
	action_id Selected_i = ID_None, Selected_j = ID_None;
	for (action_id i = 0; i < ID_MaxID; ++i)
	{
		for (action_id j = 0; j < ID_MaxID; ++j)
		{
			if (Children[i][j] == nullptr)
			{
				continue;
			}
			float Q_C = UCB1(Children[i][j]->Q, Children[i][j]->N);
			if (Q_C == INFINITY)
			{
				return Children[i][j]->Play();
			}
			if (Q_CMax < Q_C)
			{
				Q_CMax = Q_C;
				Selected_i = i;
				Selected_j = j;
			}

		}
	}
	if (Selected_i == ID_None)
	{
		Q = -1.0f;
		return -1;
	}
	return Children[Selected_i][Selected_j]->Play();
}

void opponent_node::Expansion()
{
	for (action_id i = 0; i < ID_MaxID; ++i)
	{
		if (!ParentNode->Stage.CanAction(i, Team_2P, 0))
		{
			continue;
		}
		for (action_id j = 0; j < ID_MaxID; ++j)
		{
			action_id ID[] = { i,j };
			if (!ParentNode->Stage.CanAction(ID, Team_1P))
			{
				continue;
			}

			Children[i][j] = new friend_node(this, ParentNode->Stage, NumTurns - 1);
			action_id IntentionIDs[NumTeams][stage::NumAgents] = {
				{FriendIntentionIDs[0],FriendIntentionIDs[1]},{ID[0],ID[1]}
			};
			if (!Children[i][j]->Stage.CanAction(IntentionIDs))
			{
				Children[i][j]->Q = -1.0f;
			}
			Children[i][j]->Stage.Action(IntentionIDs);
			NumChildren++;
		}
	}
}

void opponent_node::ClearChildNode()
{
	for (action_id i = 0; i < ID_MaxID; ++i)
	{
		for (action_id j = 0; j < ID_MaxID; ++j)
		{
			if (Children[i][j] == nullptr)
			{
				continue;
			}
			delete Children[i][j];
			Children[i][j] = nullptr;
		}
	}
}

opponent_node::opponent_node(friend_node *ParentNode, action_id(&Intentions)[stage::NumAgents], unsigned char NumTurns)
	:node(NumTurns)
{
	this->ParentNode = ParentNode;
	for (action_id i = 0; i < ID_MaxID; ++i)
	{
		for (action_id j = 0; j < ID_MaxID; ++j)
		{
			Children[i][j] = nullptr;
		}
	}
	for (char a = 0; a < stage::NumAgents; ++a)
	{
		FriendIntentionIDs[a] = Intentions[a];
	}
}

opponent_node::~opponent_node()
{
	ClearChildNode();
}
