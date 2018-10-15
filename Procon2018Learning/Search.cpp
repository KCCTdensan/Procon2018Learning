#include "Search.hpp"
#include "Random.hpp"
#include <iostream>
//#include <iomanip>


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
		//std::cout << "Sel=" << Ret << std::endl;
	}
	else
	{
		Ret = Evaluation();
		//std::cout << "Ev=" << Ret << std::endl;
	}
	N++;
	Record += Ret;
	Q = (float)Record / N;
	//std::cout << "Q=" << Q << "Record=" << Record << "N=" << N <<std::endl;
	return Ret;
}

int node::Selection() //子ノードのコスト関数とQ値に基づいて子ノードを選択する
{
	float Q_CMax = -10.0f;
	action_id Selected_i = None, Selected_j = None;
	for(action_id i = 0; i < Max_ActionID; ++i)
	{
		for(action_id j = 0; j < Max_ActionID; ++j)
		{
			if(Child[i][j] == nullptr)
			{
				continue;
			}
			float Q_C = UCB1(Child[i][j]->Q, Child[i][j]->N);
			if(Q_C == INFINITY)
			{
				return Child[i][j]->Play();
			}
			if(Q_CMax < Q_C)
			{
				Q_CMax = Q_C;
				Selected_i = i;
				Selected_j = j;
			}

		}
	}
	//std::cout << std::setw(4) << (int)Selected_i << "," << std::setw(4) << (int)Selected_j << ":" << Child[Selected_i][Selected_j]->Q << std::endl;
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
			NewNode->Stage.Action(Intentions, Team);
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
			for(team_no t = 0; t < NumTeams; ++t)
			{
				for(char a = 0; a < stage::NumAgents; ++a)
				{
					Intentions[t][a] = (action_id)rand() % Max_ActionID;
				}
			}
		} while(!Stage.CanAction(Intentions));
		Stage.Action(Intentions);
	}

	if(Stage.GetScore1P() > Stage.GetScore2P())
	{
		//std::cout << "win" << std::endl;
		return 1;
	}
	if(Stage.GetScore1P() == Stage.GetScore2P())
	{
		//std::cout << "draw" << std::endl;
		return 0;
	}
	//std::cout << "lose" << std::endl;
	return -1;
}

float node::UCB1(float Q, int NChild)
{
	const static float Cp = 10.0f;
	if(NChild == 0)
	{
		return INFINITY;
	}
	if(Team == Team_2P)
	{
		Q = -Q;
	}
	return Q + Cp * std::sqrtf(2.0f * std::logf((float)N) / (float)NChild);
}

bool node::IsLeafNode()
{
	return NumChildren == 0;
}

void node::ClearChildNode()
{
	for(action_id i = 0; i < Max_ActionID; ++i)
	{
		for(action_id j = 0; j < Max_ActionID; ++j)
		{
			if(Child[i][j] == nullptr)
			{
				continue;
			}
			delete Child[i][j];
			Child[i][j] = nullptr;
		}
	}
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
	ClearChildNode();
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
		N++;
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
		std::cout << std::endl;
	}

	using namespace std;
	cout << "Ns : " << N << endl;
	for(action_id i = 0; i < Max_ActionID; ++i)
	{
		for(action_id j = 0; j < Max_ActionID; ++j)
		{
			if(Child[i][j] == nullptr)
			{
				cout << " ********  ";
				continue;
			}
			printf("[N:%6d] ", Child[i][j]->N);
		}
		cout << endl;
		for(action_id j = 0; j < Max_ActionID; ++j)
		{
			if(Child[i][j] == nullptr)
			{
				cout << " ********  ";
				continue;
			}
			printf("[R:%6d] ", Child[i][j]->Record);
		}
		cout << endl << endl;
	}
}

node* node::Deepen(action_id Action1, action_id Action2)
{
	node *Ret = Child[Action1][Action2];
	Ret->Parent = nullptr;
	Child[Action1][Action2] = nullptr;
	return Ret;
}