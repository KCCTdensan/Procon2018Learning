#include <iostream>
#include <random>
#include<cmath>
#include "Search.hpp"


int node::Play() //閾値以上ならノードを展開、閾値未満ならrollout、Q値を更新
{
	if(N >= Threshold)
	{
		if(IsLeafNode())
		{
			Expansion();
		}
		return Selection();
	}
	return Evaluation();
}

int node::Selection() //子ノードのコスト関数とQ値に基づいて子ノードを選択する
{
	float Q_CMax = -1.0;
	int Selected_i = 0, Selected_j = 0;
	for(action_id i = 0; i < Max_ActionID; ++i)
	{
		for(action_id j = 0; j < Max_ActionID; ++j)
		{
			if(Child[i][j] == nullptr)
			{
				continue;
			}
			float Q_C = Child[i][j]->Q + Child[i][j]->Cost(N);
			if(Q_CMax < Q_C)
			{
				Q_CMax = Q_C;
				Selected_i = i;
				Selected_j = j;
			}
		}
	}
	return Child[Selected_i][Selected_j]->Play();
}

void node::Expansion()
{
	intention Intentions[stage::NumAgents];

	for(action_id i = 0; i < 17; ++i)
	{
		Intentions[0] = i;
		if(!Stage.CanAction(Intentions[0], Team, 0))
		{
			continue;
		}
		for(action_id j = 0; j < 17; ++j)
		{
			Intentions[1] = j;
			if(!Stage.CanAction(Intentions[1], Team, 1))
			{
				continue;
			}

			node *NewNode = new node(this, Stage, (Team == Team_1P) ? Team_2P : Team_1P);
			if(Team == Team_1P)
			{
				NewNode->IntentionID1 = i;
				NewNode->IntentionID2 = j;
				NewNode->Stage.Action(Intentions, Team);
			}
			else
			{
				NewNode->IntentionID1 = None;
				NewNode->IntentionID2 = None;
				NewNode->Stage.Action(Intentions, Team);
			}
			Child[i][j] = NewNode;
			NumChildren++;
		}
	}
}

int node::Evaluation()
{
	int Result = Rollout(Stage, stage::MaxTurn - Stage.GetCntTurn());
	/*switch(Result)
	{
	case -1:
		NumLose++;
		break;

	case 1:
		NumWin++;
		break;
	}*/
	N++;
	Record += Result;
	Q = (float)Record / N;
	return Result;
}

int node::Rollout(stage Stage, int turn)//ランダムに手を最後まで打って勝敗を返す
{
	for(int i = turn; i > 0; i--)
	{
		intention Intentions[2][2];
		bool results[2][2];
		do
		{
			for(int i = 0; i < 4; ++i)
			{
				/*Intentions[i / 2][i % 2].DeltaX = rand() % 3 - 1;
				Intentions[i / 2][i % 2].DeltaY = rand() % 3 - 1;
				if (rand() % 2)
				{
					Intentions[i / 2][i % 2].Action = IA_MoveAgent;
				}else
				{
					Intentions[i / 2][i % 2].Action = IA_RemovePanel;
				}*/
				Intentions[i / 2][i % 2] = (action_id)rand() % Max_ActionID;
			}
			Stage.CanAction(Intentions, results);
		} while(!(results[0][0] && results[0][1] && results[1][0] && results[1][1]));
		Stage.Action(Intentions);
	}

	if(Stage.GetScore1P() > Stage.GetScore2P())
	{
		return 1;
	}
	if(Stage.GetScore1P() == Stage.GetScore2P())
	{
		return 0;
	}
	return -1;
}

float node::Cost(int Ns) //このノードを選ぶのにかかるコストを返す。Alpha参照。
{
	return std::sqrtf(2.0 * std::logf(Ns)) / N;
}

bool node::IsLeafNode()
{
	return NumChildren == 0;
}

node::node(node *Parent, stage &Stage, team_no Team)
{
	this->Parent = Parent;
	NumChildren = 0;
	for(action_id i = 0; i < Max_ActionID; ++i)
	{
		for(action_id j = 0; j < Max_ActionID; ++j)
		{
			Child[i][j] = nullptr;
		}
	}
	this->Stage = Stage;
	Q = 0.0;
	N = 0;
	Record = 0;
	//NumWin = 0;
	//NumLose = 0;
	this->Team = Team;
	IntentionID1 = None;
	IntentionID2 = None;
}

node::~node()
{
	for(action_id i = 0; i < Max_ActionID; ++i)
	{
		for(action_id j = 0; j < Max_ActionID; ++j)
		{
			if(Child[i][j] != nullptr)
			{
				delete Child[i][j];
				Child[i][j] = nullptr;
			}
		}
	}
}

void node::Search(int NumCallPlay, int(&Result)[Max_ActionID][Max_ActionID])
{
	if(IsLeafNode())
	{
		Expansion();
	}
	for(int i = 0; i < NumCallPlay; ++i)
	{
		Selection();
	}
	for(action_id i = 0; i < Max_ActionID; ++i)
	{
		for(action_id j = 0; j < Max_ActionID; ++j)
		{
			if(Child[i][j] == nullptr)
			{
				Result[i][j] = 0;
				continue;
			}
			Result[i][j] = Child[i][j]->N;
		}
	}
}