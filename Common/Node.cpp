#include "Node.hpp"
#include "Random.hpp"
#include <iostream>


const static float Cp = 1.0f;
unsigned char node::NumTurns = 0;

bool node::IsLeafNode()
{
	return NumChildren == 0;
}

node::node(unsigned char CntTurns)
{
	this->CntTurns = CntTurns;
	NumChildren = 0;
	Q = 0.0;
	N = 0;
	Record = 0;
}

node::~node()
{

}

void node::ChangeNumTurns(unsigned char NumTurns)
{
	node::NumTurns = NumTurns;
}

void node::ChangeCntTurns(unsigned char CntTurns)
{
	this->CntTurns = CntTurns;
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
	if (NumTurn > 10)
	{
		NumTurn = 10;
	}
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

float friend_node::UCB1(float Q, int NChild)
{
	if (NChild == 0)
	{
		return INFINITY;
	}
	return Q + Cp * std::sqrtf(2.0f * std::logf((float)N) / (float)NChild);
}

friend_node::friend_node(opponent_node *ParentNode, stage &Stage, unsigned char NumTurns)
	:node(NumTurns), Stage(Stage)
{
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

void friend_node::Search(int NumCallPlay)
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
}

void friend_node::Result(int(&Result)[ID_MaxID][ID_MaxID])
{
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
}

opponent_node* friend_node::ChildNode(action_id IntentionID1, action_id IntentionID2)
{
	return Children[IntentionID1][IntentionID2];
}

opponent_node* friend_node::ChildNode(action_id(&IntentionIDs)[stage::NumAgents])
{
	return ChildNode(IntentionIDs[0], IntentionIDs[1]);
}

friend_node* friend_node::UpdateCurrentNode(action_id(&SelectIntentionIDs)[NumTeams][stage::NumAgents])
{
	opponent_node *SelectedOpponentNode = Children[SelectIntentionIDs[Team_1P][0]][SelectIntentionIDs[Team_1P][1]];
	friend_node *SelectedFriendNode = SelectedOpponentNode->Children[SelectIntentionIDs[Team_2P][0]][SelectIntentionIDs[Team_2P][1]];
	//SelectedOpponentNode->Children[SelectIntentionIDs[Team_2P][0]][SelectIntentionIDs[Team_2P][1]] = nullptr;
	//delete this;
	return SelectedFriendNode;
}

void friend_node::PrintStage()
{
	Stage.PrintStage();
}

void friend_node::PrintChildNodeInfo()
{
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

int opponent_node::Play()
{
	if (IsLeafNode())
	{
		Expansion();
	}
	int Ret = Selection();
	N++;
	Record += Ret;
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
			if (!ParentNode->Stage.CanAction(ID, Team_2P))
			{
				continue;
			}

			action_id IntentionIDs[NumTeams][stage::NumAgents] = {
				{FriendIntentionIDs[0],FriendIntentionIDs[1]},{ID[0],ID[1]}
			};
			if (!ParentNode->Stage.CanAction(IntentionIDs))
			{
				continue;
			}
			Children[i][j] = new friend_node(this, ParentNode->Stage, NumTurns - 1);
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

float opponent_node::UCB1(float Q, int NChild)
{
	if (NChild == 0)
	{
		return INFINITY;
	}
	return -Q + Cp * std::sqrtf(2.0f * std::logf((float)N) / (float)NChild);
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

void opponent_node::Search(int NumCallPlay)
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
}

void opponent_node::Result(int(&Result)[ID_MaxID][ID_MaxID])
{
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
}

friend_node* opponent_node::ChildNode(action_id IntentionID1, action_id IntentionID2)
{
	return Children[IntentionID1][IntentionID2];
}

friend_node* opponent_node::ChildNode(action_id(&IntentionIDs)[stage::NumAgents])
{
	return ChildNode(IntentionIDs[0], IntentionIDs[1]);
}

void opponent_node::PrintChildNodeInfo()
{
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
