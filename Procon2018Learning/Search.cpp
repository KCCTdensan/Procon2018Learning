#include "Search.hpp"
#include "Random.hpp"
#include <iostream>

int node::Play() //閾値以上ならノードを展開、閾値未満ならrollout、Q値を更新
{
	int Ret;
	if(N >= Threshold)
	{
		if(IsLeafNode())
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

int node::Selection() //子ノードのコスト関数とQ値に基づいて子ノードを選択する
{
	float Q_CMax;
	//相手のターンの場合は次は味方のターンなので、最もQ値(自分側の勝率)の高いノードを選択する。
	//味方のターンの場合は次は相手のターンなので、最もQ値(自分側の勝率)の低いノードを選択する。
	Q_CMax = (Team == Team_2P) ? 10.0f : -10.0f;
	int Selected_i = -1, Selected_j = -1;
	for(action_id i = 0; i < Max_ActionID; ++i)
	{
		for(action_id j = 0; j < Max_ActionID; ++j)
		{
			if(Child[i][j] == nullptr)
			{
				continue;
			}
			float Q_C = Child[i][j]->Q + Child[i][j]->Cost(N);
			if(Q_CMax < Q_C&& Team == Team_2P)
			{
				Q_CMax = Q_C;
				Selected_i = i;
				Selected_j = j;
			}
			else if(Q_CMax > Q_C&& Team == Team_1P)
			{
				Q_CMax = Q_C;
				Selected_i = i;
				Selected_j = j;
			}

		}
	}
	if(Selected_i == -1)
	{
		return 0;
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
				NewNode->Stage.Action(Intentions, Team);
			}
			else
			{
				NewNode->Stage.Action(Intentions, Team);
			}
			Child[i][j] = NewNode;
			NumChildren++;
		}
	}
}

int node::Evaluation()
{
	return Rollout(Stage, stage::MaxTurn - Stage.GetCntTurn());
}

int node::Rollout(stage &Stage, int NumTurn)//ランダムに手を最後まで打って勝敗を返す
{
	if(Team == Team_1P)
	{
		intention Intentions[NumTeams];
		Stage.Action(Intentions, Team_2P);
	}
	for(int i = 0; i < NumTurn; ++i)
	{
		intention Intentions[NumTeams][stage::NumAgents];
		do
		{
			for(int j = 0; j < 4; ++j)
			{
				Intentions[i / 2][i % 2] = (action_id)rand() % Max_ActionID;
			}
		} while(!Stage.CanAction(Intentions));
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
	return std::sqrtf(2.0f * std::logf((float)Ns)) / (float)N;
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
	this->Team = Team;
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

node* node::Deepen(action_id Action1, action_id Action2)
{
	for(action_id i = 0; i < Max_ActionID; ++i)
	{
		for(action_id j = 0; j < Max_ActionID; ++j)
		{
			if(i != Action1 || j != Action2)
			{
				delete Child[i][j];
				Child[i][j] = nullptr;
			}
		}
	}
	Child[Action1][Action2]->Parent = nullptr;
	return Child[Action1][Action2];
}
