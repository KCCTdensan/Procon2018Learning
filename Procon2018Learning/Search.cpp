#include <iostream>
#include <random>
#include<cmath>
#include "Search.hpp"


node::node(node *Parent, stage &Stage)
{
	this->Parent = Parent;
	this->Stage = Stage;
}

node::~node()
{
	int NumChild = (int)Child.size();
	for(int i = 0; i < NumChild; ++i)
	{
		delete Child[i];
		Child[i] = nullptr;
	}
	Child.clear();
}

void node::Search(int NumCallPlay, float(&Result)[Max_ActionID * Max_ActionID])
{
	for(int i = 0; i < NumCallPlay; ++i)
	{
		Play();
	}
}

void node::Play() //閾値以上ならノードを展開、閾値未満ならrollout、Q値を更新
{
	if(N >= Threshold)
	{
		if(IsLeafNode())
		{
			Expansion();
			return;
		}
		Selection();
		return;
	}
	switch(Rollout(Stage, stage::MaxTurn - Stage.GetCntTurn()))
	{
	case -1:
		NumLose++;
		break;

	case 1:
		NumWin++;
		break;
	}
	N++;
	Q = (float)(NumWin - NumLose) / N;
}

void node::Selection() //子ノードのコスト関数とQ値に基づいて子ノードを選択する
{
	int Q_Cmax = Child[0]->Q + Child[0]->Cost(N), selected = 0;
	for(int i = 1; i < Child.size(); i++)
	{
		if(Q_Cmax < Child[i]->Q + Child[i]->Cost(N))
		{
			Q_Cmax = Child[i]->Q + Child[i]->Cost(N);
			selected = i;
		}
	}
	Child[selected]->Play();
}

float node::Cost(int Ns) //このノードを選ぶのにかかるコストを返す。Alpha参照。
{
	return std::sqrtf(2.0 * std::logf(Ns)) / N;
}

void node::Expansion()
{
	intention Intentions[stage::NumAgents];

	for (action_id i = 0; i < 17; ++i)
	{
		bool Dummy = false;
		Intentions[0] = i;
		if(!Stage.CanAction(Intentions[0], Team, 0))
		{
			Dummy = true;
		}
		for (action_id j = 0; j < 17; ++j)
		{
			Intentions[1] = j;
			if(!Stage.CanAction(Intentions[1], Team, 1))
			{
				Dummy = true;
			}

			node *NewNode = new node(this, Stage);
			if (Team == Team_1P)
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
			NewNode->Team = (Team == Team_1P) ? Team_2P : Team_1P;
			if(Dummy)
			{
				NewNode->Q = -1.0;
			}
			Child.push_back(NewNode);
		}
	}
}

bool node::IsLeafNode()
{
	return Child.size() == 0;
}

int node::Rollout(stage Stage, int turn)//ランダムに手を最後まで打って勝敗を返す
{
	for(int i = turn; i > 0; i--)
	{
		intention Intentions[2][2];
		bool results[2][2];
		do{
			for (int i = 0; i < 4; ++i)
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
		} while (!(results[0][0]&&results[0][1]&&results[1][0]&&results[1][1]));
		Stage.Action(Intentions);
	}

	if (Stage.GetScore1P() > Stage.GetScore2P())
	{
		return 1;
	}
	if (Stage.GetScore1P() == Stage.GetScore2P())
	{
		return 0;
	}
	return - 1;
	
}